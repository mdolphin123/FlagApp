#include <WiFi.h>


//wifi stuff
const char *ssid = "DukeOpen";
const char *password = "";
unsigned long previousMillis = 0;



// Create a server on port 80
WebServer server(80);

// Variable to store the HTTP request
String header;
String output26State = "down";  // Flag down
String output27State = "off";   // Another GPIO control if needed



// Motor and button pins
const int motorPin1 = 16; // Motor control pin for clockwise
const int motorPin2 = 17; // Motor control pin for counterclockwise
const int enablePin = 33; //enablePin
const int sensorPin = 34; //sensorPin

//Extra variables
int exceededCurrent;
int initialized = 1;
int RaiseStallCurrent;
int LowerStallCurrent;
float elapsedTime = 0;
int currentSum = 0;
int currentThreshold;
int averageCurrent;
float flagHeight;
float percent;
float RaiseExpectedTime = 1.3;
float LowerExpectedTime = 1.1;
float shortHeight;
int raiseInit = 0;
int lowerInit = 0;
float shortTime;
float tempTime;
float tempHeight;
int currentReading;
int sshort = 0;
// Variable to store the current flag state ("half mast" or "full mast")
String flagState = "unknown";  // Start with the flag at half mast
String flagStatus = "flagStatus";
String instance = "";
String enteredPassword = "";

// Current measurement variables
float current_value;
float sensor_multiplier = 0.185; // Sensor sensitivity in V/A (e.g., ACS712 5A version)

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 100000;
const String correctPassword = "goduke";


void setup() {

 Serial.begin(9600);
 Serial.println("ESP32 is running!");
 //WiFi.softAP("ESP32");

 // Set motor pins as output
 pinMode(motorPin1, OUTPUT);
 pinMode(motorPin2, OUTPUT);
 pinMode(4, OUTPUT);

 // Ensure both motor control pins are off
 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, LOW);

 // Connecting to WiFi
 Serial.print("Connecting to ");
 Serial.println(ssid);
WiFi.begin(ssid, password);
WiFi.setSleep(false);

while (WiFi.status() != WL_CONNECTED) {
   delay(10);
   Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected.");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());

 // Start the server
 server.on("/", handleRoot);
 server.on("/get", HTTP_GET, handleGet);
 server.on("/post", HTTP_POST, handlePost, handleUpload);

 server.begin();

 digitalWrite(4, HIGH);

}

void initialize() {
  initialized = 1;
  raiseInit = 0;
  lowerInit = 0;
  flagStatus = "needs to be initialized";
}

void initialize_raise() {
  elapsedTime = 0;
  currentSum = 0;
  instance = "raise";
  raiseInit = 1;
   
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
  flagStatus = "initialized raising";
            
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(enablePin, LOW);
  currentSum = 0;
}

void initialize_lower() {
  instance = "lower";
  lowerInit = 1;
       
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
  
  int averageCurrent = currentSum / 20;
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
  flagStatus = "initialized lowering";
             
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(enablePin, LOW);
  currentSum = 0;
}


int raiseflag() {
  elapsedTime = 0;
  currentSum = 0;

  //initialize stuff
  instance = "raise";
  raiseInit = 1;
  Serial.println("Raising to full mast...");

  //raise the flag here
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(enablePin, HIGH);
  delay(20);
  for (int i = 0; i < 20; i++) {
    currentSum += analogRead(sensorPin);
    elapsedTime += 0.01;
    delay(10);
  }

  //use current sensor to raise
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

  //time elapsed
  RaiseExpectedTime =  elapsedTime;
  Serial.println("Current: " + String(exceededCurrent) + " has exceeded threshold");
  flagStatus = "initialized raising";
  

  //end raising
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(enablePin, LOW);
  currentSum = 0;
  

  //check conditions
  flagHeight += elapsedTime * (220/RaiseExpectedTime);
  if (flagHeight <= 205) {
    flagState = "partial full mast";
    percent = (flagHeight/220) * 100;
    flagStatus = String(percent) + "% full mast";
    return percent;
  }
  else {
    flagHeight = 220;
    flagStatus = "Ran to Full Mast Successfully";
    flagState = "full mast";
    return 100;
  }

  if(elapsedTime > 1.15 * RaiseExpectedTime) {
    flagStatus = "exceeded max runtime, possible rope slippage";
    return -1;
  }
}

int lowerflag() {
  instance = "lower";
  lowerInit = 1;
  Serial.println("Lowering to half mast...");

  //set up motors
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(enablePin, HIGH);
  delay(20);
  for (int i = 0; i < 20; i++) {
    currentSum += analogRead(sensorPin);
    elapsedTime += 0.01;
    delay(10); 
  }

  int averageCurrent = currentSum / 20;
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

  //time elapsed
  LowerExpectedTime = elapsedTime;
  Serial.println("Current: " + String(exceededCurrent) + " has exceeded threshold");
  flagStatus = "initialized lowering";
             
  //end lowering
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(enablePin, LOW);
  currentSum = 0;
  
  //run tests
  flagHeight -= elapsedTime * (220/LowerExpectedTime);
  if (flagHeight >= 15) {
    flagState = "partial half mast";
    percent = ((220-flagHeight)/220)*100;
    flagStatus = String(percent) + "% half mast";
    return percent;
  }
  else {
    flagHeight = 0;
    flagStatus = "Ran to Half Mast Successfully";
    flagState = "half mast";
    return 100;
  }

  if(elapsedTime > 1.15 * LowerExpectedTime) {
    flagStatus = "exceeded max runtime, possible rope slippage";
    return -1;
  }

}



void loop() {
  server.handleClient();

  unsigned long currentMillis = millis();
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=30000)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }
}


void handleRoot() {
  server.send(200, "text/plain", "Ready");
}

void handleGet() {
  if (server.hasArg("data")) {
    String data = server.arg("data");
    int value = 0;
    if(data.equals("UP")) {
      value = raiseflag();
    }
    else {
      value = lowerflag();
    }
    //client.print("received");
    server.send(200, "text/plain", value);
  }
  
}

void handlePost() {
  server.send(200, "text/plain", "Processing Data");
}


void handleUpload() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    Serial.println("Receiving data:");
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    Serial.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    server.send(200, "text/plain", "Data: ");
  }
}


void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();
  server.handleClient();
}
