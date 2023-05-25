#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <PubSubClient.h>
//#include <WiFi.h>
#include <ESP8266WiFi.h>

#define argsLimit 5
#define sensorNumber 1
#define jsonBuffer 256
#define sensorTrigger 3500

const char* uuid = "trackOccupancyHub0";
// Replace the next variables with your SSID/Password combination
const char* ssid = "Modelljernbane";
const char* password = "mat.tab.tea";

// Add your MQTT Broker IP address:
const char* mqtt_server = "192.168.0.219";

WiFiClient espClient;
PubSubClient client(espClient);

//Initialize sensor pins
int pins[sensorNumber] = {D2};
int ids[sensorNumber] = {0};

struct sensor{
  uint8_t pin;
  uint16_t value;
  uint16_t last;
  uint8_t id;
};
sensor Sensors[sensorNumber];

//TaskHandle_t controller;
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
    Sensors[i].pin = pins[i];
    Sensors[i].id = ids[i];
  }
  //Run motor controll on separate core
  //xTaskCreatePinnedToCore(
  //  Run,
  //  "Track Occupancy",
  //  4096,
  //  NULL,
  //  1,
  //  NULL,
  //  0
  //);
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
  for(int i = 0; i < sensorNumber; i++){
      
      //values are in range 4096 to 0
      Sensors[i].value = readQD(Sensors[i].pin);
      Serial.print(" Pin: ");
      Serial.print(Sensors[i].pin);
      Serial.print(" Value: ");
      Serial.print(Sensors[i].value);

      //Detected train if value goes LOWER than threshold
      if (Sensors[i].value == HIGH && Sensors[i].last == LOW){
        //Going from detecting a train to not detecting a train
        send(Sensors[i].id, LOW);
      }
      else if(Sensors[i].value == LOW && Sensors[i].last == HIGH){
        //Going form NOT detecting a train to detecting a train
        send(Sensors[i].id, HIGH);        
      }
      Sensors[i].last = Sensors[i].value;
    }
    //vTaskDelay(100);
    Serial.println("");
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
      Sensors[i].value = digitalRead(Sensors[i].pin);
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
    //vTaskDelay(100);
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

int readQD(int pin){
  //Returns value from the QRE1113
  //Lower numbers mean more refleacive
  //More than 3000 means nothing was reflected.
  pinMode( pin, OUTPUT );
  digitalWrite( pin, HIGH );
  delayMicroseconds(10);
  pinMode( pin, INPUT );
  long time = micros();
  //time how long the input is HIGH, but quit after 3ms as nothing happens after that
  while (digitalRead(pin) == HIGH && micros() - time < 3000);
  int diff = micros() - time;
  return diff > 2500U;
}

void reconnect() {
  Serial.print("Attempting MQTT connection...");
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (client.connect(uuid)) {
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
