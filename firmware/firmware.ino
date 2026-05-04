#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D8
#define RST_PIN D0
#define BUZZER_PIN D4
#define LED_ON_PIN D2
#define LED_READ_PIN D3

const char* ssid = "Mobile_AP";
const char* password = "frd4aws2";
const char* mqtt_server = "137.131.151.245";

WiFiClient espClient;
PubSubClient client(espClient);

MFRC522 rfid(SS_PIN, RST_PIN);

// -------- BUZZER --------
void beep() {
  tone(BUZZER_PIN, 950);
  digitalWrite(LED_READ_PIN, HIGH); // 2kHz
  delay(200);            // duração do beep
  noTone(BUZZER_PIN);
  delay(200); 
  digitalWrite(LED_READ_PIN, LOW);
}

// ------------------------

void conectarWifi() {

  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    digitalWrite(LED_ON_PIN, HIGH);
    Serial.print(".");
    delay(250);
    digitalWrite(LED_ON_PIN, LOW);
  }

  digitalWrite(LED_ON_PIN, HIGH);
  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void conectarMQTT() {

  while (!client.connected()) {

    Serial.println("Conectando ao broker MQTT...");

    if (client.connect("esp8266_rfid")) {
      Serial.println("MQTT conectado!");
    }
    else {

      Serial.print("Erro MQTT: ");
      Serial.println(client.state());

      Serial.println("Tentando novamente...");
      delay(2000);
    }
  }
}

void setup() {

  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_ON_PIN, OUTPUT);
  pinMode(LED_READ_PIN, OUTPUT);

  conectarWifi();

  client.setServer(mqtt_server, 1883);

  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Leitor RFID pronto.");
}

void loop() {

  if (!client.connected()) {
    conectarMQTT();
  }

  client.loop();

  if (!rfid.PICC_IsNewCardPresent())
    return;

  if (!rfid.PICC_ReadCardSerial())
    return;

  Serial.println("\nCartão detectado!");

  // 🔊 beep ao ler cartão
  beep();

  String uidString = "";

  for (byte i = 0; i <rfid.uid.size; i++) {

    if (rfid.uid.uidByte[i] < 0x10)
      uidString += "0";

    uidString += String(rfid.uid.uidByte[i], HEX);
  }

  Serial.print("UID do cartão: ");
  Serial.println(uidString);

  boolean ok = client.publish("sensor/UND001/rfid", uidString.c_str());

  if (ok) {
    Serial.println("UID enviado para MQTT");
  } else {
    Serial.println("Falha ao enviar UID");
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(2000);
}
