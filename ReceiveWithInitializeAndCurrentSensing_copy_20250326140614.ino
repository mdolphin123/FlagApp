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
const int motorPin1 = 26;  // Pin to run the motor forward (raise flag)
const int motorPin2 = 14;  // Pin to run the motor backward (lower flag)
const int enablePin = 33;
const int sensorPin = 34;
float RaiseExpectedTime = 1.3;
float LowerExpectedTime = 1.1;
int exceededCurrent = 0;
bool initialized = false;
int RaiseStallCurrent = 0;
int LowerStallCurrent = 0;
float elapsedTime = 0;
int currentSum = 0;
int currentThreshold = 0;
int averageCurrent = 0;


int currentReading = 0;








void setup() {
 Serial.begin(115200);
  Serial.println("Setup started");

  Serial.println("Setting up motor pins...");
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(enablePin, LOW);

  Serial.println("Initializing WiFi...");
  WiFi.begin(ssid);

  Serial.println("Waiting for WiFi connection...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected.");

  Serial.println("Setting up MQTT...");
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnectMQTT();

  Serial.println("Setup complete.");
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



void initialize()
{
elapsedTime = 0;
  currentSum = 0;
  
   
    
     Serial.println("Raising to full mast...");


     digitalWrite(motorPin1, HIGH);
     digitalWrite(motorPin2, LOW);
     digitalWrite(enablePin, HIGH);
     delay(20);
     for (int i = 0; i < 20; i++) {
         currentSum += analogRead(sensorPin);
         elapsedTime += 0.01;
         delay(10);
        
     }


     int averageCurrent = currentSum / 20;
     currentThreshold = averageCurrent * 1.05;
     RaiseStallCurrent = currentThreshold;

         while (true) {
          currentReading = analogRead(sensorPin);
             if (currentReading > currentThreshold) {
                 exceededCurrent = currentReading;
                 break;
             }
             Serial.print("Current: ");
             Serial.println(analogRead(sensorPin));
             delay(10);
             elapsedTime += 0.01;
             
             }
             RaiseExpectedTime =  elapsedTime;
             Serial.println("Current: " + String(exceededCurrent) + " has exceeded threshold");
            
            
             digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, LOW);
     digitalWrite(enablePin, LOW);
      currentSum = 0;
      elapsedTime = 0;
      delay(200);
         
         
             
        
         
          
       
   
     Serial.println("Lowering to half mast...");


     digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, HIGH);
     digitalWrite(enablePin, HIGH);
     delay(20);
     for (int i = 0; i < 20; i++) {
         currentSum += analogRead(sensorPin);
         elapsedTime += 0.01;
         delay(10);
        
     }


      averageCurrent = currentSum / 20;
     currentThreshold = averageCurrent * 1.05;
     LowerStallCurrent = currentThreshold;

         while (true) {
              currentReading = analogRead(sensorPin);
             if (currentReading > currentThreshold) {
                 exceededCurrent = currentReading;
                 break;
             }
             Serial.print("Current: ");
             Serial.println(analogRead(sensorPin));
             delay(10);
             elapsedTime += 0.01;
             
             }
             LowerExpectedTime = elapsedTime;
             Serial.println("Current: " + String(exceededCurrent) + " has exceeded threshold");
           
             
             digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, LOW);
     digitalWrite(enablePin, LOW);
      currentSum = 0;
      elapsedTime = 0;
     initialized = true;
     client.publish(statusTopic, "init");
}


void raiseFlag() {
 elapsedTime = 0;
     Serial.println("Raising to full mast...");


     digitalWrite(motorPin1, HIGH);
     digitalWrite(motorPin2, LOW);
     digitalWrite(enablePin, HIGH);
     delay(100);
     elapsedTime += 0.1;
         while (true) {
            Serial.println(RaiseStallCurrent);
            currentReading = analogRead(sensorPin);
             if (currentReading >= RaiseStallCurrent) {
              digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, LOW);
     digitalWrite(enablePin, LOW);
                 exceededCurrent = currentReading;
                 break;
             }
             Serial.print("Current: ");
             Serial.println(analogRead(sensorPin));
             delay(10);
             elapsedTime += 0.01;
             if (elapsedTime > (1.1 * RaiseExpectedTime)) {
              digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, LOW);
     digitalWrite(enablePin, LOW);
                client.publish(statusTopic, "exceededmaxruntime");
                
                 break;
             }
         }
       
        
    
     currentSum = 0;
     client.publish(statusTopic, "raised");
}


void lowerFlag() {
 elapsedTime = 0;
   
     Serial.println("Lowering to half mast...");


     digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, HIGH);
     digitalWrite(enablePin, HIGH);
     delay(100);
     elapsedTime += 0.1;
         while (true) {
          Serial.println(LowerStallCurrent);
          currentReading = analogRead(sensorPin);
    
             if (currentReading >= LowerStallCurrent) {
              digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, LOW);
     digitalWrite(enablePin, LOW);
                 exceededCurrent = currentReading;
                 break;
             }
             Serial.print("Current: ");
             Serial.println(analogRead(sensorPin));
             delay(10);
             elapsedTime += 0.01;
             if (elapsedTime > (1.1 * LowerExpectedTime)) {
              digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, LOW);
     digitalWrite(enablePin, LOW);
                client.publish(statusTopic, "exceededmaxruntime");
               
                 break;
             }
         }
       

         currentSum = 0;
         client.publish(statusTopic, "lowered");
}


void stopMotor() {
 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(enablePin, LOW);
}

void callback(char* topic, byte* payload, unsigned int length) {
 String command;
 for (unsigned int i = 0; i < length; i++) {
   command += (char)payload[i];
 }
 Serial.println("Command Received: " + command);

if (command == "init") initialize();
 if (command == "raise") raiseFlag();
 if (command == "lower") lowerFlag();
}
void loop() {
 if (!client.connected()) {
   reconnectMQTT();
 }
 client.loop();
}