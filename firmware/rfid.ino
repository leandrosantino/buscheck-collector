#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define SS_PIN D8
#define RST_PIN D0
#define BUZZER_PIN D4
#define GPS_RX_PIN D3  // GPS TX -> D1 (RX do ESP)
#define GPS_TX_PIN D2  // GPS RX -> D2 (TX do ESP)

const char* ssid = "Click.com";
const char* password = "comum104";
const char* mqtt_server = "10.0.0.105";

WiFiClient espClient;
PubSubClient client(espClient);

MFRC522 rfid(SS_PIN, RST_PIN);

// Objetos GPS
SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN);
TinyGPSPlus gps;

// Variáveis para controle de tempo do GPS
unsigned long lastGPSTime = 0;
const unsigned long GPS_INTERVAL = 10000; // 10 segundos
unsigned long lastValidGPSTime = 0;
bool gpsHasValidFix = false;

// -------- BUZZER --------
void beep() {
  tone(BUZZER_PIN, 950); // 2kHz
  delay(200);            // duração do beep
  noTone(BUZZER_PIN);
}

// -------- GPS --------
void processarGPS() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
}

void exibirInfoGPS() {
  int satelites = gps.satellites.value();
  Serial.print("Satélites encontrados: ");
  Serial.println(satelites);

  if (gps.location.isValid()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.print(gps.altitude.meters(), 2);
    Serial.println(" m");
    Serial.print("Velocidade: ");
    Serial.print(gps.speed.kmph(), 2);
    Serial.println(" km/h");

    if (gps.date.isValid() && gps.time.isValid()) {
      Serial.print("Data/Hora: ");
      Serial.print(gps.date.year());
      Serial.print("-");
      Serial.print(gps.date.month());
      Serial.print("-");
      Serial.print(gps.date.day());
      Serial.print(" ");
      Serial.print(gps.time.hour());
      Serial.print(":");
      Serial.print(gps.time.minute());
      Serial.print(":");
      Serial.println(gps.time.second());
    }
  } else {
    Serial.println("Localização: Inválida (aguardando fixação)");

    if (gpsHasValidFix) {
      unsigned long secondsSinceFix = (millis() - lastValidGPSTime) / 1000;
      Serial.print("Última fixação válida há: ");
      Serial.print(secondsSinceFix);
      Serial.println(" segundos");
    }
  }
}

void enviarLocalizacao() {
  int satelites = gps.satellites.value();
  String gpsData = "";

  if (gps.location.isValid()) {
    // Formato quando localização é válida: lat,lng,alt,vel,satelites
    gpsData = String(gps.location.lat(), 6) + "," +
              String(gps.location.lng(), 6) + "," +
              String(gps.altitude.meters(), 2) + "," +
              String(gps.speed.kmph(), 2) + "," +
              String(satelites);

    Serial.print("GPS: Localização válida - ");

    // Atualizar tempo da última fixação válida
    lastValidGPSTime = millis();
    gpsHasValidFix = true;
  } else {
    // Formato quando localização não é válida: 0,0,0,0,satelites
    gpsData = "0,0,0,0," + String(satelites);

    Serial.print("GPS: Aguardando fixação - ");
  }

  Serial.print(satelites);
  Serial.println(" satélite(s) encontrado(s)");

  boolean ok = client.publish("sensor/UND001/gps", gpsData.c_str());

  if (ok) {
    Serial.println("Dados GPS enviados para MQTT");
    Serial.print("Dados: ");
    Serial.println(gpsData);
  } else {
    Serial.println("Falha ao enviar dados GPS");
  }

  // Exibir informações detalhadas para debug
  exibirInfoGPS();
}

// ------------------------

void conectarWifi() {

  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

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
  gpsSerial.begin(9600);  // Inicializar comunicação serial com GPS

  pinMode(BUZZER_PIN, OUTPUT);

  conectarWifi();

  client.setServer(mqtt_server, 1883);

  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Leitor RFID pronto.");
  Serial.println("Aguardando sinal GPS...");
}

void loop() {

  if (!client.connected()) {
    conectarMQTT();
  }

  client.loop();

  // Processar dados do GPS continuamente
  processarGPS();

  // Enviar localização a cada intervalo definido
  unsigned long currentTime = millis();
  if (currentTime - lastGPSTime >= GPS_INTERVAL) {
    enviarLocalizacao();
    lastGPSTime = currentTime;
  }

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


