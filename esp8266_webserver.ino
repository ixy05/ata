#include <ESP8266WiFi.h>
 
const char* ssid = "ssid";//type your ssid
const char* password = "password";//type your password
 
int contactSensor = 2; // GPIO2 of ESP8266
int trigger = 0;
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  
  pinMode(contactSensor, INPUT_PULLUP);
  //pinMode(trigger, OUTPUT);
  //digitalWrite(trigger, LOW);
   
  // Connect to WiFi network
  Serial.println();
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
   
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
    
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
   
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
   
  // Match the request
 
 int value = digitalRead(contactSensor);


 /*
  if (request.indexOf("/setState?value=1") != -1) {
    digitalWrite(trigger, HIGH);
    delay(2000);
    digitalWrite(trigger, LOW);
    delay(2000);
    digitalWrite(trigger, HIGH);
  } 
  if (request.indexOf("/setState?value=0") != -1){
    digitalWrite(trigger, HIGH);
    delay(2000);
    digitalWrite(trigger, LOW);
    delay(2000);
    digitalWrite(trigger, HIGH);
  }
*/
// Set ledPin according to the request
digitalWrite(trigger, value);
   
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
 // client.println("<!DOCTYPE HTML>");
 // client.println("<html>");
   
   
  if(value == LOW) {
    client.print("{"); 
    client.print("\"currentState\": 1");  
    client.print("}"); 
  } else {
    client.print("{"); 
    client.print("\"currentState\": 0");  
    client.print("}");
  }
  //client.println("</html>");
 
  delay(10);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
