import sqlite3
import base64
from datetime import datetime
import paho.mqtt.client as mqtt 
import json

con = sqlite3.connect('Database\db.sqlite')
cur = con.cursor()

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
        data = json.loads(str(msg.payload.decode("utf-8")))
        q = f'INSERT into motorValue (motorId, target, time, sender) values ({data["Id"]}, {data["Speed"]}, "{datetime.now()}", "{data["Sender"]}");'
        print(q)
        cur.execute(q)
        con.commit()
    elif str(msg.topic) == "switch/cmd/state":
        data = json.loads(str(msg.payload.decode("utf-8")))
        t = 1 if data["State"] else 0
        q = f'INSERT into switchValue (switchId, target, time, sender) values ({data["Id"]}, {t}, "{datetime.now()}", "{data["Sender"]}");'
        print(q)
        cur.execute(q)
        con.commit()
    elif str(msg.topic) == "Sensors/Reading":
        data = json.loads(str(msg.payload.decode("utf-8")))
        q = f'INSERT into sensorValue (sensorId, value, time) values ({data["Id"]}, {data["Value"]}, "{datetime.now()}");'
        print(q)
        cur.execute(q)
        con.commit()
    else:
        message_bytes = str(msg.payload.decode('ascii')).encode('ascii')
        base64_bytes = base64.b64encode(message_bytes)
        q = f'INSERT into misc (message, time) values ("{base64_bytes.decode("ascii")}", "{datetime.now()}");'
        print(q)
        cur.execute(q)
        con.commit()

def on_log(client, userdata, level, buf):
    print("log: ",buf)

client = mqtt.Client(clean_session=True, userdata=None, transport='websockets')
client.on_connect = on_connect
client.on_message = on_message
client.on_log=on_log

client.connect(host = "localhost", port=9001, keepalive=60)
client.loop_forever()

con.close()