#include <analogWrite.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Motor.h>

constexpr std::uint8_t argsLimit = 5U;
constexpr std::uint8_t trackLimit = 12U;
constexpr std::uint16_t jsonBuffer = 256U;

// Give a unique identifier in the system
const char* uuid = "MotorController";

// Replace the next variables with your SSID/Password combination
const char* ssid = "Modelljernbane";
const char* password = "mat.tab.tea";

// Add your MQTT Broker IP address:
const char* mqtt_server = "192.168.0.219";
WiFiClient espClient;
PubSubClient client(espClient);

//Motor
Motor RailwaySystem[trackLimit];
TaskHandle_t controller;
//Timer
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000U;  //the value is a number of milliseconds

void setup() {
  //Serial interface for debugging
  Serial.begin(9600U);
  //MQTT related setup
  setup_wifi();
  client.setServer(mqtt_server, 1883U);
  client.setCallback(callback);
  
  //Configure motors here:
  RailwaySystem[0].setup(5, 4);
  
  //Run motor controll on separate core
  xTaskCreatePinnedToCore(
    Run,
    "MotorControl",
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

void Run(void* p){
  //process all motor outputs
  while(1){    
    for(int i = 0; i < trackLimit; i++){
      //Any tracks without pins return to avoid wasting time. 
      RailwaySystem[i].Run();
      vTaskDelay(1);
    }
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
    int Speed             = payload["Speed"];
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
  Serial.print("Attempting MQTT connection...");
  //Turn off motors
  for (int i = 0; i < trackLimit; i++){
    RailwaySystem[i].setSpeed(0);
  }
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (client.connect(uuid)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("motor/cmd/speed");
      client.subscribe("motor/cmd");
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
