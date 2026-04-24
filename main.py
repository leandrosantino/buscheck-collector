import os
from dotenv import load_dotenv

import paho.mqtt.client as mqtt
import json
import paho.mqtt.client as mqtt
from flask import Flask
from supabase import create_client, Client

load_dotenv()

url: str = os.environ.get("SUPABASE_URL")
key: str = os.environ.get("SUPABASE_KEY")

supabase: Client = create_client(url, key)

MQTT_BROKER = os.environ.get("MQTT_BROKER")
MQTT_PORT = int(os.environ.get("MQTT_PORT"))
MQTT_TOPIC = "sensor/+/rfid"

# ---------- Flask ----------
app = Flask(__name__)

# ---------- MQTT ----------
def on_connect(client, userdata, flags, rc):
    print("MQTT conectado:", rc)
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
    topic_parts = msg.topic.split("/")
    device_id = topic_parts[1]
    payload = msg.payload.decode()

    print(f"[MQTT] {device_id} -> {payload}")

    try:
        response = (
            supabase.table("check_in")
            .insert({
                "codigo_veiculo": device_id,
                "codigo_cartao": payload,
            })
            .execute()
        )
        print(response)
    except Exception as e:
        print(e)


mqtt_client = mqtt.Client()
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message

mqtt_client.connect(MQTT_BROKER, MQTT_PORT)
mqtt_client.loop_start()


# ---------- START ----------

if __name__ == "__main__":
    print("Servidor iniciado")
    app.run(host="0.0.0.0", port=5000, debug=True)
