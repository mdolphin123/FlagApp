


// Load Wi-Fi library
#include <WiFi.h>


// Replace with your network credentials
const char* ssid = "Verizon XT1565 8311";
//const char* username = "mql3";
const char* password = "Chase100!";


// Set web server port number to 80
WiFiServer server(80);


// Variable to store the HTTP request
String header;


// Auxiliary variables to store the current output state
String output26State = "down";  // Flag down
String output27State = "off";   // Another GPIO control if needed


// Assign output variables to GPIO pins for the motor control
const int motorPin1 = 26;  // Pin to run the motor forward (raise flag)
const int motorPin2 = 14;  // Pin to run the motor backward (lower flag)
const int enablePin = 33;
const int sensorPin = 34;


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


// Authentication state
bool isAuthenticated = false;  // Initially set to false
IPAddress authenticatedClientIP;  // Store the IP of the authenticated client


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
 Serial.begin(115200);


 // Initialize the motor control pins as outputs
 pinMode(motorPin1, OUTPUT);
 pinMode(motorPin2, OUTPUT);
 pinMode(enablePin, OUTPUT);


 // Set both motor control pins to LOW initially (motor off)
 digitalWrite(motorPin1, LOW);
 digitalWrite(motorPin2, LOW);
 digitalWrite(enablePin, LOW);


 // Connect to Wi-Fi network with SSID and password
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
void initialize()
{
  initialized = 1;
  raiseInit = 0;
  lowerInit = 0;
  flagStatus = "needs to be initialized";
}
void init(String action1)
{
   elapsedTime = 0;
  currentSum = 0;
     if (action1 == "raise") {
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
         
             
         else if (action1 == "lower")
         {
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
    
    
 
}
void handlePostRequest(String action) {
  
 if (action == "raise") {
   instance = "raise";
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
                flagStatus = "exceeded max runtime";
                 break;
             }
         }
       
         if (flagStatus != "exceeded max runtime") 
         {
             Serial.println("Current: " + String(exceededCurrent) + " has exceeded threshold");
            
             
         } 
     
    
     currentSum = 0;

 } else if (action == "lower") {
    elapsedTime = 0;
    instance = "lower";
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
     
                flagStatus = "exceeded max runtime";
                 break;
             }
         }
       
         if (flagStatus != "exceeded max runtime") 
         {
             Serial.println("Current: " + String(exceededCurrent) + " has exceeded threshold");
             
             
             
         } 
         currentSum = 0;
     }
    
     

 }





void loop() {
 WiFiClient client = server.available();  // Listen for incoming clients


 if (client) {  // If a new client connects
     currentTime = millis();
     previousTime = currentTime;
     Serial.println("New Client.");
     String currentLine = "";  // Make a String to hold incoming data from the client
     bool isPostRequest = false;
     String postData = "";  // Store POST data


     while (client.connected() && currentTime - previousTime <= timeoutTime) {  // Loop while the client's connected
         currentTime = millis();


         if (client.available()) {  // If there's bytes to read from the client
             char c = client.read();  // Read a byte
             Serial.write(c);  // Print it out the serial monitor
             header += c;


             if (c == '\n') {  // If the byte is a newline character
                 if (currentLine.length() == 0) {  // Headers have been received
                     if (isPostRequest) {  // If it's a POST request, read the body data
                         while (client.available()) {
                             postData += (char)client.read();
                         }


                         // Check if the password is correct
                         if (postData.indexOf("password=" + correctPassword) >= 0) {
                             isAuthenticated = true;
                             authenticatedClientIP = client.remoteIP();  // Store the authenticated client's IP
                             Serial.println("Authenticated successfully. Client IP: " + authenticatedClientIP.toString());
                         } else if (header.indexOf("POST /raise") >= 0 && isAuthenticated && client.remoteIP() == authenticatedClientIP) {
                             handlePostRequest("raise");
                         } else if (header.indexOf("POST /lower") >= 0 && isAuthenticated && client.remoteIP() == authenticatedClientIP) {
                             handlePostRequest("lower");
                         } else {
                             Serial.println("Incorrect password or unauthorized action.");
                             client.println("<script>alert('Incorrect password or unauthorized action.');</script>");
                         }
                     }


                     client.println("HTTP/1.1 200 OK");
                     client.println("Content-type:text/html");
                     client.println("Connection: close");
                     client.println();
                     if(header.indexOf("POST /raise1") >= 0)
                     {
                      init("raise");
                      
                     }
                     else if(header.indexOf("POST /lower1") >= 0)
                     {
                        init("lower");
                        
                     }
  else if (header.indexOf("POST /raise") >= 0) {
                      
                        handlePostRequest("raise");
                    } else if (header.indexOf("POST /lower") >= 0) {
                      
                        handlePostRequest("lower");
                    }
                    else if(header.indexOf("POST /initialize") >=0)
                    {
                      initialize();
          
                    }


                     // HTML Web Page Generation
                 client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
client.println("<style>");
client.println("body { font-family: Arial, sans-serif; text-align: center; background-color: #f4f4f9; margin: 0; }");
client.println(".flag-container { position: relative; width: 50px; height: 500px; margin: 20px auto; background-color: #888; }");
client.println(".flagpole { position: absolute; width: 10px; height: 100%; background: #333; left: 20px; top: 0; }");
client.println(".flag { position: absolute; width: 150px; height: 60px; background: red; border: 1px solid #000; }");
client.println(".flag.hidden { display: none; }");
client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 20px; margin: 10px; cursor: pointer; border-radius: 5px; }");
client.println("</style>");
client.println("</head>");
client.println("<body>");
client.println("<h1>ESP32 Flag Controller</h1>");

if (!isAuthenticated || client.remoteIP() != authenticatedClientIP) {
    client.println("<p>Enter password to unlock controls:</p>");
    client.println("<form action='/login' method='POST'>");
    client.println("<input type='password' name='password' placeholder='Enter password'><br><br>");
    client.println("<button type='submit' class='button'>Submit</button>");
    client.println("</form>");
} else {
  if(raiseInit == 0 && lowerInit == 0)
  {
    flagStatus = "needs to be initialized";
    client.println("<div class='flag-container'>");
    client.println("<div class='flagpole'></div>");
    
        client.println("<div class='flag hidden'></div>"); // Hidden if state is unknown
    
    client.println("</div>");
    
    client.println("<p>Status: " + flagStatus + "</p>");
    client.println("<p>Initialize raise and lower instances</p>");
    client.println("<form action='/raise1' method='post'><button class='button'>Raise to Full Mast</button></form>");
    client.println("<form action='/lower1' method='post'><button class='button'>Lower to Half Mast</button></form>");
    client.println("</body></html>");
  }
  else if(raiseInit == 1 && lowerInit == 0)
  {
    flagHeight = 220;
    flagState = "full mast";
    flagStatus = "initialized raising, please initialize lowering";
    client.println("<div class='flag-container'>");
    client.println("<div class='flagpole'></div>");
     client.println("<div class='flag' style='top: 0;'></div>");
    
  
    
    client.println("</div>");
       client.println("<p>The flag is currently at: <strong>" + flagState + "</strong></p>");
    client.println("<p>Status: " + flagStatus + "</p>");
    client.println("<p>Initialized  Raising Expected Time: " + String(RaiseExpectedTime) + "</p>");
    
    client.println("<p>Initialized  Raising Stall current: " + String(RaiseStallCurrent) + "</p>");
    
      client.println("<p>Exceeded Current: " + String(exceededCurrent) + "</p>");
    client.println("<p>Initialize lower instance</p>");
    client.println("<form action='/raise1' method='post'><button class='button'>Raise to Full Mast</button></form>");
    client.println("<form action='/lower1' method='post'><button class='button'>Lower to Half Mast</button></form>");
    client.println("<form action='/initialize' method='post'><button class='button'>Reinitialize</button></form>");

    client.println("</body></html>");
  }
  else if(raiseInit == 0 && lowerInit == 1)
  {
    flagHeight = 0;
    flagState = "half mast";
    flagStatus = "initialized lowering, please initialize raising";
    client.println("<div class='flag-container'>");
    client.println("<div class='flagpole'></div>");
     client.println("<div class='flag' style='top: 220px;'></div>");
    
  
    
    client.println("</div>");
    client.println("<p>The flag is currently at: <strong>" + flagState + "</strong></p>");
    client.println("<p>Status: " + flagStatus + "</p>");
    client.println("<p>Initialized Lowering Expected Time: " + String(LowerExpectedTime) + "</p>");
    
    client.println("<p>Initialized  Lowering Stall current: " + String(LowerStallCurrent) + "</p>");
      client.println("<p>Exceeded Current: " + String(exceededCurrent) + "</p>");
    client.println("<p>Initialize raise instance</p>");
    client.println("<form action='/raise1' method='post'><button class='button'>Raise to Full Mast</button></form>");
    
    client.println("<form action='/lower1' method='post'><button class='button'>Lower to Half Mast</button></form>");
    client.println("<form action='/initialize' method='post'><button class='button'>Reinitialize</button></form>");

    client.println("</body></html>");
  }

  else if(raiseInit == 1 && lowerInit == 1){
    if(initialized == 1)
    {
      flagStatus = "initialized";
      client.println("<div class='flag-container'>");
    client.println("<div class='flagpole'></div>");
    if (instance == "raise") {
      flagState = "full mast";
        client.println("<div class='flag' style='top: 0;'></div>");
        flagHeight = 220;
    } else if (instance == "lower") {
        flagState = "half mast";
        client.println("<div class='flag' style='top: 220px;'></div>");
        flagHeight = 0;
    } 
    client.println("</div>");
    
    client.println("<p>The flag is currently at: <strong>" + flagState + "</strong></p>");
    client.println("<form action='/raise' method='post'><button class='button'>Raise to Full Mast</button></form>");
    client.println("<form action='/lower' method='post'><button class='button'>Lower to Half Mast</button></form>");
    client.println("<form action='/initialize' method='post'><button class='button'>Reinitialize</button></form>");

    client.println("<p>Status: " + flagStatus + "</p>");
    client.println("<p>Initialized  Raising Expected Time: " + String(RaiseExpectedTime) + "</p>");
    client.println("<p>Initialized Lowering Expected Time: " + String(LowerExpectedTime) + "</p>");
    client.println("<p>Initialized  Raising Stall current: " + String(RaiseStallCurrent) + "</p>");
    client.println("<p>Initialized  Lowering Stall current: " + String(LowerStallCurrent) + "</p>");
      client.println("<p>Exceeded Current: " + String(exceededCurrent) + "</p>");
      if(instance == "raise")
      {
        client.println("<p>Last Instance : Raise </p>");
      }
      else if(instance == "lower")
      {
        client.println("<p>Last Instance : Lower </p>");
      }
        
        client.println("<p>Runtime: " + String(elapsedTime) + " seconds</p>");
        initialized = 2;
    }
    else{
   
    if((instance =="raise" && flagState == "full mast") || (instance == "lower" && flagState == "half mast"))
    {
       client.println("<div class='flag-container'>");
    client.println("<div class='flagpole'></div>");
    if (instance == "raise") {
      flagState = "full mast";
        client.println("<div class='flag' style='top: 0;'></div>");
        flagHeight = 220;
        flagStatus = "flag was already at full mast";
    } else if (instance == "lower") {
        flagState = "half mast";
        flagStatus = "flag was already at half mast";
        client.println("<div class='flag' style='top: 220px;'></div>");
        flagHeight = 0;
    } 
    client.println("</div>");
    
    client.println("<p>The flag is currently at: <strong>" + flagState + "</strong></p>");
    client.println("<form action='/raise' method='post'><button class='button'>Raise to Full Mast</button></form>");
    client.println("<form action='/lower' method='post'><button class='button'>Lower to Half Mast</button></form>");
    client.println("<form action='/initialize' method='post'><button class='button'>Reinitialize</button></form>");

    client.println("<p>Status: " + flagStatus + "</p>");
    client.println("<p>Initialized  Raising Expected Time: " + String(RaiseExpectedTime) + "</p>");
    client.println("<p>Initialized Lowering Expected Time: " + String(LowerExpectedTime) + "</p>");
    client.println("<p>Initialized  Raising Stall current: " + String(RaiseStallCurrent) + "</p>");
    client.println("<p>Initialized  Lowering Stall current: " + String(LowerStallCurrent) + "</p>");
    
    client.println("<p>Exceeded Current: " + String(exceededCurrent) + "</p>");
      if(instance == "raise")
      {
        client.println("<p>Last Instance : Raise </p>");
      }
      else if(instance == "lower")
      {
        client.println("<p>Last Instance : Lower </p>");
      }
        
        client.println("<p>Runtime: " + String(elapsedTime) + " seconds</p>");
    }
    else
    {
    if(instance == "raise")
    {
      flagHeight += elapsedTime * (220/RaiseExpectedTime);
      if (flagHeight <= 205)
      {
        flagState = "partial full mast";
        percent = (flagHeight/220) * 100;
        flagStatus = String(percent) + "% full mast";
      }
      else
      {
        flagHeight = 220;
        flagStatus = "Ran to Full Mast Successfully";
        
        flagState = "full mast";
      }
    }
    else if(instance == "lower")
    {
      flagHeight -= elapsedTime * (220/LowerExpectedTime);
      if (flagHeight >= 15)
      {
        flagState = "partial half mast";
        percent = ((220-flagHeight)/220)*100;
        flagStatus = String(percent) + "% half mast";
      }
      else
      {
        flagHeight = 0;
        flagStatus = "Ran to Half Mast Successfully";
        
        flagState = "half mast";
      }
    }
    if(instance == "raise")
    {
      if(elapsedTime > 1.15 * RaiseExpectedTime)
      {
        flagStatus = "exceeded max runtime, possible rope slippage";
      }
    }
    else if(instance == "lower")
    {
      if(elapsedTime > 1.15 * LowerExpectedTime)
      {
      flagStatus = "exceeded max runtime, possible rope slippage";
      }
    }
     client.println("<div class='flag-container'>");
    client.println("<div class='flagpole'></div>");
    tempHeight = 220 - flagHeight;
  
      client.println("<div class='flag' style='top: " + String(tempHeight) + "px;'></div>");

       
    client.println("</div>");
    
    client.println("<p>The flag is currently at: <strong>" + flagState + "</strong></p>");
    client.println("<form action='/raise' method='post'><button class='button'>Raise to Full Mast</button></form>");
    client.println("<form action='/lower' method='post'><button class='button'>Lower to Half Mast</button></form>");
    client.println("<form action='/initialize' method='post'><button class='button'>Reinitialize</button></form>");
    client.println("<p>Status: " + flagStatus + "</p>");
    client.println("<p>Initialized  Raising Expected Time: " + String(RaiseExpectedTime) + "</p>");
    client.println("<p>Initialized Lowering Expected Time: " + String(LowerExpectedTime) + "</p>");
    client.println("<p>Initialized  Raising Stall current: " + String(RaiseStallCurrent) + "</p>");
    client.println("<p>Initialized  Lowering Stall current: " + String(LowerStallCurrent) + "</p>");
      client.println("<p>Exceeded Current: " + String(exceededCurrent) + "</p>");
    client.println("<p>Runtime: " + String(elapsedTime) + " seconds</p>");
      if(instance == "raise")
      {
      
        client.println("<p>Last Instance : Raise </p>");
        if(flagState == "partial full mast")
        {
          client.println("<p>Runtime less than expected, try raising flag again</p>");
        }

      }
      else if(instance == "lower")
      {
        client.println("<p>Last Instance : Lower </p>");
         if(flagState == "partial half mast")
        {
          client.println("<p>Runtime less than expected, try lowering flag again</p>");
        }
      }
        
        
       
          client.println("</body></html>");
    }
  
  
    
    

  
  
}
  }
  }

                     client.println();
                     break;
                 } else {
                     currentLine = "";
                 }
             } else if (c != '\r') {
                 currentLine += c;
                 if (currentLine.startsWith("POST /login")) {  // Check if the request is a POST
                     isPostRequest = true;
                 }
             }
         }
     }
     header = "";
     client.stop();
     Serial.println("Client disconnected.");
 }
}



