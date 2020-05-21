/*
Spencer Waguespack
5/19/2020
arduino code to convert rectelinear acceloration into vactor sum
*/


#include <ArduinoJson.h>
#include<Wire.h>

const int n = 100;
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int16_t ax[n],ay[n],az[n],gx[n],gy[n],gz[n];
const size_t capacity = JSON_OBJECT_SIZE(6);
DynamicJsonDocument doc(capacity);

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(2000000);

}

void loop(){
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

  AcX=rollingavg(ax,n,AcX);
  AcY=rollingavg(ay,n,AcY);
  AcZ=rollingavg(az,n,AcZ);
  GyX=rollingavg(gx,n,GyX);
  GyY=rollingavg(ax,n,GyY);
  GyZ=rollingavg(ax,n,GyZ);

  Serial.println(vec_sum(AcX,AcY,AcZ));
  
//  doc["AcX"]=AcX;    
//  doc["AcY"]=AcY;  
//  doc["AcZ"]=AcZ;  
//  doc["GyX"]=GyX;  
//  doc["GyY"]=GyY; 
//  doc["GyZ"]=GyZ;
//  
//  serializeJson(doc, Serial);
//  Serial.println(" ");

  delay(5);
}

int16_t rollingavg(int16_t senval[],uint8_t n, int16_t newval){
  int16_t acc =0;
  for(int i = 0; i<n-1;i++){
     senval[i] = senval[i+1];
     acc = senval[i+1]+acc;
   }
   acc =acc+newval;
   senval[n] = newval;
   return acc/n;
}

int16_t vec_sum(uint16_t x,uint16_t y,uint16_t z){
  int16_t vec_sum = sqrt(x*x+y*y+z*z);
  return vec_sum;
}
