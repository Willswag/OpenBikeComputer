#include<Wire.h>

#include <ArduinoJson.h>

const int led = 13;
const int MPU=0x68; 

int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

String outdata;

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
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);

  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();
  
  doc["wheel_delt"]=rw_delt;
  doc["AcX"]=AcX;    
  doc["AcY"]=AcY;  
  doc["AcZ"]=AcZ;  
  doc["GyX"]=GyX;  
  doc["GyY"]=GyY; 
  doc["GyZ"]=GyZ;
  
  serializeJson(doc, Serial);
  Serial.println(" ");
}

void rrpm(){
  unsigned long temp = micros();
  rw_delt = temp - tim;
  tim = temp;
  }
