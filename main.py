import os
from dotenv import load_dotenv
import paho.mqtt.client as mqtt
from supabase import create_client, Client

load_dotenv()

SUPABASE_URL: str = os.environ.get("SUPABASE_URL")
SUPABASE_KEY: str = os.environ.get("SUPABASE_KEY")
MQTT_BROKER = os.environ.get("MQTT_BROKER")
MQTT_PORT = int(os.environ.get("MQTT_PORT"))
MQTT_TOPIC = "sensor/+/rfid"

supabase: Client = create_client(SUPABASE_URL, SUPABASE_KEY)

def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code.is_failure:
        print("⚠️ MQTT Connection", reason_code)
    else:
        print("✅ MQTT Connection", reason_code, "\n")
        client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
    topic_parts = msg.topic.split("/")
    device_id = topic_parts[1]
    payload = msg.payload.decode()

    print(f"🟦 [MQTT] {device_id} -> {payload}")

    try:
        response = (
            supabase.table("check_in")
            .insert({
                "codigo_veiculo": device_id,
                "codigo_cartao": payload,
            })
            .execute()
        )
        print("🟩 Success send to database", '\n')
    except Exception as e:
        print("🟥 Failure to send database", '\n')

if __name__ == "__main__":
    mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message
    mqtt_client.connect(MQTT_BROKER, MQTT_PORT)

    print("🌍 MQTT subscriber service started!!")
    mqtt_client.loop_forever()
