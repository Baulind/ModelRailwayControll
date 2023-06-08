#include <WiFi.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <PubSubClient.h>

#define argsLimit 5
#define lightLimit 9
#define lightOffsett 0
#define jsonBuffer 256

// Give unit a unique identifier in the system.
const char* uuid = "intersectionLightsTest";

// Replace the next variables with your SSID/Password combination
const char* ssid = "Modelljernbane";
const char* password = "mat.tab.tea";

// Add your MQTT Broker IP address:
const char* mqtt_server = "192.168.0.219";

WiFiClient espClient;
PubSubClient client(espClient);

struct light{
  uint8_t redPin;
  uint8_t greenPin;
  bool state;
   
  light(uint8_t _redPin, uint8_t _greenPin)
  {
    redPin = _redPin;
    greenPin = _greenPin;
  }
  light(){}
};

light lights[lightLimit] = {
  light(16, 17),
  light(15, 2),
  light(0, 4),
  light(22, 23),
  light(5, 18),
  light(26, 27),
  light(14, 12),
  light(33, 25)
};

void setup() {
  //Serial interface for debugging
  Serial.begin(9600);
  //MQTT related setup
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  for(int i = 0; i < lightLimit; i++){
    pinMode(lights[i].redPin, OUTPUT);
    pinMode(lights[i].greenPin, OUTPUT);
  }
}

void loop() {
  //Make sure we have a server connection
  if (!client.connected()) {
    reconnect();
  }
  //Check for messages
  client.loop();
  for(int i = 0; i < lightLimit; i++){
    //Serial.print("R_Pin: ");
    //Serial.print(lights[i].redPin);
    //Serial.print(" R_Val: ");
    //Serial.print(!lights[i].state);
    //Serial.print(" G_Pin: ");
    //Serial.print(lights[i].redPin);
    //Serial.print(" G_Val: ");
    //Serial.println(lights[i].state);
    digitalWrite(lights[i].redPin, !lights[i].state);
    digitalWrite(lights[i].greenPin, lights[i].state);
  }
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  //Reserve memory
  StaticJsonDocument<jsonBuffer> payload;
  deserializeJson(payload, message, length);

  Serial.print("Message arrived on topic: ");
  Serial.println(topic);
  
  if (String(topic) == "light/cmd/state") {
    uint8_t Id                = payload["Id"];
    Id -= lightOffsett;
    bool State                = payload["State"];
    //Do something with ligh
    Serial.print("Id: ");
    Serial.print(Id);
    Serial.print(" State: ");
    Serial.println(State);
    if (Id < lightLimit)
      lights[Id].state = State;

  }
  else if(String(topic) == "light/cmd"){
    //Handle cmd
  }
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(uuid)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("light/cmd/state");
      client.subscribe("light/cmd");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 second");
      // Wait 1 second before retrying
      delay(1000);
    }
  }
}
