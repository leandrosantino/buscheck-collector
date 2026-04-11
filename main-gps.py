import paho.mqtt.client as mqtt
import json
import paho.mqtt.client as mqtt
from flask import Flask, render_template
from flask_socketio import SocketIO, join_room

MQTT_BROKER = "10.0.0.105"
MQTT_PORT = 1883
MQTT_TOPIC = "sensor/+/rfid"

# ---------- Flask ----------

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

@app.route("/socket")
def home():
    return render_template("index.html")

# ---------- MQTT ----------

def on_connect(client, userdata, flags, rc):
    print("MQTT conectado:", rc)
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
    topic_parts = msg.topic.split("/")
    device_id = topic_parts[1]
    payload = msg.payload.decode()

    # lat, long = str(payload).split("/")
    # lat, long = float(lat), float(long)

    print(f"[MQTT] {device_id} -> {payload}")

    # envia apenas para quem está escutando esse device
    socketio.emit(
        "esp_data",
        {
            "device_id": device_id,
            "user_uid": payload,
        },
        room=device_id
    )

mqtt_client = mqtt.Client()
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message

mqtt_client.connect(MQTT_BROKER, MQTT_PORT)
mqtt_client.loop_start()

# ---------- SOCKETIO ----------

@socketio.on("listen_device")
def listen_device(data):
    device_id = data["device"]
    print("Cliente escutando:", device_id)
    join_room(device_id)

# ---------- START ----------

if __name__ == "__main__":
    print("Servidor SocketIO iniciado")
    socketio.run(app, host="0.0.0.0", port=5000)
