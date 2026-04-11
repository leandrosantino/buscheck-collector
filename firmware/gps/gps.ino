#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

const char* ssid = "Click.com";
const char* password = "comum104";

const char* mqtt_server = "10.0.0.105";

WiFiClient espClient;
PubSubClient client(espClient);

// GPS
TinyGPSPlus gps;
SoftwareSerial gpsSerial(D5, D6); // RX, TX

unsigned long lastSend = 0;

void conectarWifi() {

  Serial.println("Conectando ao WiFi...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void conectarMQTT() {

  while (!client.connected()) {

    Serial.println("Conectando ao broker MQTT...");

    if (client.connect("esp8266_gps")) {
      Serial.println("MQTT conectado!");
    } else {

      Serial.print("Falha na conexão MQTT. Código: ");
      Serial.println(client.state());

      Serial.println("Tentando novamente...");
      delay(2000);
    }
  }
}

void setup() {

  Serial.begin(9600);
  gpsSerial.begin(9600);

  Serial.println("Inicializando ESP8266...");

  conectarWifi();

  client.setServer(mqtt_server, 1883);

  Serial.print("Servidor MQTT configurado: ");
  Serial.println(mqtt_server);
}

void loop() {

  if (!client.connected()) {
    conectarMQTT();
  }

  client.loop();

  // leitura contínua do GPS
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  // envia a cada 5 segundos
  if (millis() - lastSend > 5000) {

    if (gps.location.isValid()) {

      double lat = gps.location.lat();
      double lng = gps.location.lng();

      String payload = String(lat, 6) + "/" + String(lng, 6);

      Serial.print("GPS: ");
      Serial.println(payload);

      boolean ok = client.publish("sensor/UND001/gps", payload.c_str());

      if (ok) {
        Serial.println("Mensagem enviada com sucesso!");
      } else {
        Serial.println("Falha ao enviar mensagem!");
      }

    } else {
      Serial.println("Aguardando sinal GPS...");
    }

    Serial.println("-------------------------");

    lastSend = millis();
  }
}