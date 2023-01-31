#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";

// Const input and output pins: 
const int input1; 
const int output1; 

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  //Initialize inputs: 
    //What do we take as an input and which pins? 
    //pinMode() & digitalWrite()


  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            

            //Headline: 
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            //Define slider with slider box: 
            client.println(<style> .slidecontainer {width: 100%;})
            client.println(.slider {-webkit-appearance: none; width: 50%; height: 25px; background: #d3d3d3; outline: none; opacity: 1.7; -webkit-transition: .2s; transition: opacity .2s;})
            client.println(.slider:hover {opacity: 1;})
            client.println(.slider::-webkit-slider-thub {-webkit-appearance: none; appearance: none; width: 25px; height: 25px; background: #04AA6D; cursor: pointer;})
            client.println(.slider::-mqz-range-thumb {width: 25px; height: 25px; background: #04AA6D; cursor: pointer;})            
            client.println(</style></head><body>)
            client.println(<h1>Slider for train Controller</h1>)
            client.println(<div class = "slidecontainer"> <input type="range" min="1" max="100" value="50" class="slider" id="myRange">)
            //Display value: 
            client.println(<p>Value: <span id="demo"></span></p></div>)
            client.println(<script>var slider = document.getElementById("myRange");)
            client.println(var output = document.getElementById("demo");)
            client.println(output.innerHTML = slider.value;)
            client.println(slider.oninput = function() {output.innerHTML = this.value;} </script>)
            client.println(</body> </html>)

            //Ending the Terminal Print: 
            client.println();

            // Break out of the while loop
            break;
          } 
          else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
