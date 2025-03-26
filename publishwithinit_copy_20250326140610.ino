#include <WiFi.h>
#include <PubSubClient.h>
#include <WebServer.h>


// Wi-Fi Credentials
const char* ssid = "DukeVisitor";
const char* password = "Your_WiFi_Password";


// HiveMQ MQTT Broker
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;


WiFiClient espClient;
PubSubClient mqttClient(espClient);
WebServer server(80);


// Define flag topics
const char* flag1_control = "flagController/flag1/control";
const char* flag2_control = "flagController/flag2/control";
const char* flag3_control = "flagController/flag3/control";


// Define status topics
const char* flag1_status = "flagController/flag1/status";
const char* flag2_status = "flagController/flag2/status";
const char* flag3_status = "flagController/flag3/status";


// Store status updates
String flag1State = "Unknown";
String flag2State = "Unknown";
String flag3State = "Unknown";


void setup() {
 Serial.begin(115200);


 // Connect to Wi-Fi
 WiFi.begin(ssid);
 Serial.print("Connecting to WiFi...");
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }
 Serial.println("\nWiFi Connected. IP: " + WiFi.localIP().toString());


 // Connect to MQTT
 mqttClient.setServer(mqtt_server, mqtt_port);
 mqttClient.setCallback(mqttCallback);
 reconnectMQTT();


 // Set up web server routes
 server.on("/", HTTP_GET, handleRoot);
 server.on("/control", HTTP_POST, handleControl);  // 🚀 Changed to POST
 server.on("/status", HTTP_GET, handleStatus);


 server.begin();
}


void reconnectMQTT() {
 while (!mqttClient.connected()) {
   Serial.print("Connecting to MQTT...");
   if (mqttClient.connect("Central_Controller")) {
     Serial.println("Connected to HiveMQ!");
     mqttClient.subscribe(flag1_status);
     mqttClient.subscribe(flag2_status);
     mqttClient.subscribe(flag3_status);
   } else {
     Serial.print("Failed, retrying...");
     delay(5000);
   }
 }
}


void mqttCallback(char* topic, byte* payload, unsigned int length) {
 String message;
 for (unsigned int i = 0; i < length; i++) {
   message += (char)payload[i];
 }


 if (String(topic) == flag1_status) flag1State = message;
 if (String(topic) == flag2_status) flag2State = message;
 if (String(topic) == flag3_status) flag3State = message;


 Serial.println("Status Update: " + message);
}


void handleRoot() {
 String page = "<html><head><title>Flag Controller</title>";
  // 🚀 JavaScript for automatic updates
 page += "<script>";
 page += "setInterval(() => fetch('/status').then(r => r.text()).then(d => document.getElementById('status').innerHTML = d), 2000);";
 page += "</script></head><body>";


 page += "<h1>ESP32 Flag Controller</h1>";
  // 🚀 Display flag statuses (this will be updated dynamically)
 page += "<h2>Flag Status</h2>";
 page += "<div id='status'>";
 page += "<p>Flag 1: " + flag1State + "</p>";
 page += "<p>Flag 2: " + flag2State + "</p>";
 page += "<p>Flag 3: " + flag3State + "</p>";
 page += "</div>";


 // 🚀 Modified form to use POST instead of GET
 page += "<form action='/control' method='post'>";
 page += "<label>Select Flags to Control:</label><br>";
 page += "<input type='checkbox' name='flag' value='flag1'> Flag 1<br>";
 page += "<input type='checkbox' name='flag' value='flag2'> Flag 2<br>";
 page += "<input type='checkbox' name='flag' value='flag3'> Flag 3<br>";
 //if(initialized1 == true && initialized2 == true)
// {
 page += "<br><button name='action' value='raise'>Raise Selected Flags</button>";
 page += "<button name='action' value='lower'>Lower Selected Flags</button>";
//}
//else
//{
  page += "<br><button name='action' value='init'>Initialize Selected Flags</button>";
//}
 page += "</form></body></html>";


 server.send(200, "text/html", page);
}




// 🚀 Modified to process POST data
void handleControl() {
 if (server.hasArg("action")) {
   String action = server.arg("action");  // Extract action (raise/lower)


   if (server.hasArg("flag")) {
     Serial.println("Flags selected:");
     for (uint8_t i = 0; i < server.args(); i++) {
       if (server.argName(i) == "flag") {  // ✅ Only process "flag" arguments
         String flag = server.arg(i);
         Serial.println("Command Sent: " + action + " to " + flag);
        
         // Construct MQTT topic dynamically
         String topic = String("flagController/") + flag + "/control"; 
         Serial.println(topic);
         mqttClient.publish(topic.c_str(), action.c_str());
       }
     }
   }
  server.send(204, "text/plain", "");  
  //server.send(200, "text/html", "<h2>Command Sent!</h2><a href='/'>Back</a>");
 } else {
   server.send(400, "text/html", "<h2>Error: No action received</h2><a href='/'>Back</a>");
 }
}
void handleStatus() {
 String statusPage = "<p>Flag 1: " + flag1State + "</p>";
 statusPage += "<p>Flag 2: " + flag2State + "</p>";
 statusPage += "<p>Flag 3: " + flag3State + "</p>";
 server.send(200, "text/html", statusPage);
}


void loop() {
 if (!mqttClient.connected()) {
   reconnectMQTT();
 }
 mqttClient.loop();
 server.handleClient();
}



