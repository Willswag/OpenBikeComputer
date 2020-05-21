#include<Wire.h>

#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "ATT3zzz8Zq"
#define STAPSK  "545h7rj=8ya7"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const int led = 13;
const int MPU=0x68; 

int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

String outdata;

ESP8266WebServer server(80);

void ICACHE_RAM_ATTR rrpm();

const uint8_t rwheel = 14;//set pins for front wheel speed sensor
const uint8_t fwheel = 0;//set pins for rear wheel speed sensing
const uint8_t inpin =0;
volatile unsigned long tim = micros();
volatile unsigned long rw_delt;
float hz;

const size_t capacity = JSON_OBJECT_SIZE(7);
DynamicJsonDocument doc(capacity);

void setup(void) {
  pinMode(inpin,INPUT);
  attachInterrupt(digitalPinToInterrupt(rwheel), rrpm, FALLING);

  StaticJsonDocument<200> doc;
  doc["sensor"] = "gps";
  doc["time"] = 1351824120;
  
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

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

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  MDNS.update();
  
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);

  doc["wheel_delt"]=rw_delt;
  doc["AcX"]=Wire.read()<<8|Wire.read();    
  doc["AcY"]=Wire.read()<<8|Wire.read();  
  doc["AcZ"]=Wire.read()<<8|Wire.read();  
  doc["GyX"]=Wire.read()<<8|Wire.read();  
  doc["GyY"]=Wire.read()<<8|Wire.read();  
  doc["GyZ"]=Wire.read()<<8|Wire.read();
  
  serializeJson(doc, outdata);
  
  server.on("/acceloration", []() {
    server.send(200, "text/plain", outdata);
  });

//  server.on("/acceloration", []() {
//    server.send(200, "text/plain", "this works as well");
//  });

  
//  Serial.print("Accelerometer: ");
//  Serial.print("X = "); Serial.print(AcX);
//  Serial.print(" | Y = "); Serial.print(AcY);
//  Serial.print(" | Z = "); Serial.println(AcZ); 
//  
//  Serial.print("Gyroscope: ");
//  Serial.print("X = "); Serial.print(GyX);
//  Serial.print(" | Y = "); Serial.print(GyY);
//  Serial.print(" | Z = "); Serial.println(GyZ);
//  Serial.println(" ");
}

void rrpm(){
  unsigned long temp = micros();
  rw_delt = temp - tim;
  tim = temp;
  }

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}  
