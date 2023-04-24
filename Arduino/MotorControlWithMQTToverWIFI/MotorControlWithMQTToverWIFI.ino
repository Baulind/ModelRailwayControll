#include <analogWrite.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Motor.h>

#define argsLimit 5
#define trackLimit 12
#define jsonBuffer 256

// Replace the next variables with your SSID/Password combination
//const char* ssid = "Modelljernbane";
//const char* password = "mat.tab.tea";
const char* ssid = "Get-2G-36523F";
const char* password = "ESSEPUNG";

// Add your MQTT Broker IP address:
//const char* mqtt_server = "192.168.0.219";
const char* mqtt_server = "192.168.0.114";
WiFiClient espClient;
PubSubClient client(espClient);

//Motor
Motor RailwaySystem[trackLimit];

void setup() {
  //Serial interface for debugging
  Serial.begin(9600);
  //MQTT related setup
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //Debug motor
  RailwaySystem[0].setup(4, 16, 17, 0.1, 0.05);
}

void loop() {
  //Make sure we have a server connection
  if (!client.connected()) {
    reconnect();
  }
  //Check for messages
  client.loop();
  //process all motor outputs
  for(int i = 0; i < trackLimit; i++){
    //Any tracks without pins return to avoid wasting time. 
    RailwaySystem[i].Run();
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
  
  if (String(topic) == "motor/cmd/speed") {
    int Id                = payload["Id"];
    double Speed          = payload["Speed"];
    RailwaySystem[Id].setSpeed(map(Speed, -100, 100, -255, 255));    
  }
  else if (String(topic) == "motor/cmd") {
    int Args[argsLimit];
    int i = 0;
    int Id                = payload["Id"];
    const char* Command   = payload["Command"];
    JsonArray temp        = payload["Args"].as<JsonArray>();
    for(JsonVariant v :temp){
      Args[i] = v.as<int>();
      i++;
    }
    char* reply           = RailwaySystem[Id].handleCMD(Command, Args);    
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("MotorController")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("motor/cmd/speed");
      client.subscribe("motor/cmd");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 second");
      // Wait 1 second before retrying
      delay(1000);
    }
  }
}
