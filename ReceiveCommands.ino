#include <WiFi.h>
#include <PubSubClient.h>




const char* ssid = "DukeVisitor";
const char* password = "Your_WiFi_Password";


// HiveMQ MQTT Broker
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;


// Unique ESP32 ID 
const char* flagID = "flag1"; 


// MQTT Topics
const char* controlTopic = "flagController/flag1/control";
const char* statusTopic = "flagController/flag1/status";


WiFiClient espClient;
PubSubClient client(espClient);


// Motor Pins
const int motorPin1 = 16;
const int motorPin2 = 17;
const int enablePin = 4;


void setup() {
 Serial.begin(115200);


 // Connect to Wi-Fi
 WiFi.begin(ssid);
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }
 Serial.println("\nWiFi Connected.");


 
 client.setServer(mqtt_server, mqtt_port);
 client.setCallback(callback);
 reconnectMQTT();




 pinMode(motorPin1, OUTPUT);
 pinMode(motorPin2, OUTPUT);
 pinMode(enablePin, OUTPUT);


 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(enablePin, LOW);
}


void reconnectMQTT() {
 while (!client.connected()) {
   Serial.print("Connecting to MQTT...");
   if (client.connect(flagID)) {
     Serial.println("Connected to HiveMQ!");
     client.subscribe(controlTopic);
   } else {
     Serial.print("Failed, retrying...");
     delay(5000);
   }
 }
}


void callback(char* topic, byte* payload, unsigned int length) {
 String command;
 for (unsigned int i = 0; i < length; i++) {
   command += (char)payload[i];
 }
 Serial.println("Command Received: " + command);


 if (command == "raise") raiseFlag();
 if (command == "lower") lowerFlag();
}


void raiseFlag() {
 Serial.println("Raising Flag...");
 digitalWrite(motorPin1, HIGH);
 digitalWrite(enablePin, HIGH);
 delay(3000);
 stopMotor();
 client.publish(statusTopic, "raised");
}


void lowerFlag() {
 Serial.println("Lowering Flag...");
 digitalWrite(motorPin2, HIGH);
 digitalWrite(enablePin, HIGH);
 delay(3000);
 stopMotor();
 client.publish(statusTopic, "lowered");
}


void stopMotor() {
 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(enablePin, LOW);
}


void loop() {
 if (!client.connected()) {
   reconnectMQTT();
 }
 client.loop();
}





