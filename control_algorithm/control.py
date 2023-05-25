import paho.mqtt.client as mqtt 
import json
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe('motor/cmd/speed')
    client.subscribe('switch/cmd/state')
    client.subscribe('motor/cmd')
    client.subscribe('switch/cmd')
    client.subscribe('Sensors/Reading')

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    if str(msg.topic) == "motor/cmd/speed":
        pass
    elif str(msg.topic) == "switch/cmd/state":
        pass
    elif str(msg.topic) == "Sensors/Reading":
        pass
    else:
        pass

def on_log(client, userdata, level, buf):
    print("log: ",buf)

client = mqtt.Client(clean_session=True, userdata=None, transport='websockets')
client.on_connect = on_connect
client.on_message = on_message
client.on_log=on_log
client.connect(host = "localhost", port=9001, keepalive=60)
client.loop_forever()