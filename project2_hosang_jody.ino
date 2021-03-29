#include <ESP8266WiFi.h> 
#include <WiFiClient.h> 
#include <ESP8266WebServer.h> 
#include <ESP8266mDNS.h> 
const char* ssid = "network goes here"; 
const char* password = "password goes here"; 
#define echoPin 2 /
#define trigPin 0 
const int buzzer = 15;
const int ledPin = 13;
long duration;
int distance;
int safetyDistance;

ESP8266WebServer server(80); 

void handleRoot() {

server.send(200, "text/plain", "Ultrasonic HC-SR40 : CM : " + String(distance)); 

}
void handleNotFound(){

String message = "File Not Found\n\n";
message += "URI: ";
message += server.uri();
message += "\nMethod: ";
message += (server.method() == HTTP_GET)?"GET":"POST";
message += "\nArguments: ";
message += server.args();
message += "\n";
for (uint8_t i=0; i<server.args(); i++){
message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
}
server.send(404, "text/plain", message);

}
void setup(void){
Serial.begin(115200); 
WiFi.begin(ssid, password);
Serial.println("");
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(buzzer, OUTPUT);
pinMode(ledPin, OUTPUT);
Serial.println("IO&E Project 2");
// Wait for connection
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.print("Connected to "); 
Serial.println(ssid);
Serial.print("IP address: "); 
Serial.println(WiFi.localIP());
if (MDNS.begin("esp8266")) {
Serial.println("MDNS responder started");
}
server.on("/", handleRoot);
server.on("/inline", [](){
server.send(200, "text/plain", "this works as well");
});
server.onNotFound(handleNotFound);
server.begin();
Serial.println("HTTP server started");
}
void loop(void){
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = duration*0.034/2; 
Serial.print("Distance: ");
Serial.println(distance);
safetyDistance = distance;
if (safetyDistance <= 183){
  digitalWrite(buzzer, HIGH);
  digitalWrite(ledPin, HIGH);
}
else{
  digitalWrite(buzzer, LOW);
  digitalWrite(ledPin, LOW);
}


delay(10); 
}
