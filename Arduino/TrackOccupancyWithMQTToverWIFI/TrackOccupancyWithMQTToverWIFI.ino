#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <PubSubClient.h>

#define argsLimit 5
#define sensorNumber 3
#define jsonBuffer 256
#define sensorTrigger 3500

// Replace the next variables with your SSID/Password combination
//const char* ssid = "Modelljernbane";
//const char* password = "mat.tab.tea";
const char* ssid = "Get-2G-36523F";
const char* password = "ESSEPUNG";

// Add your MQTT Broker IP address:
const char* mqtt_server = "192.168.0.57";
//const char* mqtt_server = "192.168.0.219";

WiFiClient espClient;
PubSubClient client(espClient);

//Initialize sensor pins
int pins[sensorNumber] = {32, 34, 35};
int ids[sensorNumber] = {0, 1, 2};

struct sensor{
  uint8_t pin;
  uint16_t value;
  uint16_t last;
  uint8_t id;
};
sensor Sensors[sensorNumber];

TaskHandle_t controller;
//Timer
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;  //the value is a number of milliseconds

void setup() {
  //Serial interface for debugging
  Serial.begin(9600);
  //MQTT related setup
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);

  for (int i = 0; i < sensorNumber; i++){
    pinMode(pins[i], INPUT);
    Sensors[i].pin = pins[i];
    Sensors[i].id = ids[i];
  }
  pinMode(34, INPUT);
  //Run motor controll on separate core
  xTaskCreatePinnedToCore(
    Run,
    "Track Occupancy",
    4096,
    NULL,
    1,
    NULL,
    0
  );
  //start timer
  startMillis = millis();
}

void loop() {
  //Make sure we have a server connection
  if (!client.connected()) {
    reconnect();
  }
  //Check for messages
  client.loop();
}

void send(int id, int value){
  DynamicJsonDocument doc(jsonBuffer);
  char buffer[jsonBuffer];
  doc["Id"] = id;
  doc["Value"] = value;
  size_t n = serializeJson(doc, buffer);
  client.publish("Sensors/Reading", buffer, n);
}

void Run(void* p){
  //Match task signature
  while(1){    
    //For each sensor
    for(int i = 0; i < sensorNumber; i++){
      
      //values are in range 4096 to 0
      Sensors[i].value = analogRead(Sensors[i].pin);
      Serial.print(" Pin: ");
      Serial.print(Sensors[i].pin);
      Serial.print(" Value: ");
      Serial.print(Sensors[i].value);

      //Detected train if value goes LOWER than threshold
      if (Sensors[i].value > sensorTrigger && Sensors[i].last < sensorTrigger){
        //Going from detecting a train to not detecting a train
        send(Sensors[i].id, LOW);
      }
      else if(Sensors[i].value < sensorTrigger && Sensors[i].last > sensorTrigger){
        //Going form NOT detecting a train to detecting a train
        send(Sensors[i].id, HIGH);        
      }
      Sensors[i].last = Sensors[i].value;
    }
    vTaskDelay(100);
    Serial.println("");
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

  //else if (String(topic) == "motor/cmd") {
  //  int Args[argsLimit];
  //  int i = 0;
  //  int Id                = payload["Id"];
  //  const char* Command   = payload["Command"];
  //  JsonArray temp        = payload["Args"].as<JsonArray>();
  //  for(JsonVariant v :temp){
  //    Args[i] = v.as<int>();
  //    i++;
  //  }
  //  char* reply           = RailwaySystem[Id].handleCMD(Command, Args);    
  //}
}

void reconnect() {
  Serial.print("Attempting MQTT connection...");
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (client.connect("MotorController")) {
      Serial.println("connected");
      // Subscribe
      //client.subscribe("Sensors/cmd");
    } 
    else {
      currentMillis = millis();  
      if (currentMillis - startMillis >= period) 
      {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 1 second");
        startMillis = currentMillis;
      }
    }
  }
}
