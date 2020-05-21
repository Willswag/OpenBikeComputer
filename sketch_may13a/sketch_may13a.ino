#include<Wire.h>
#include <esp8266wifi.h>
#include <esp8266httpclient.h>

const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;


void ICACHE_RAM_ATTR rrpm();

const uint8_t rwheel = 14;//set pins for front wheel speed sensor
const uint8_t fwheel = 0;//set pins for rear wheel speed sensing
const uint8_t inpin =0;
volatile unsigned long tim = micros();
volatile unsigned long rw_delt;
float hz;

void setup() {
  Serial.begin(9600);
  pinMode(inpin,INPUT);
  attachInterrupt(digitalPinToInterrupt(rwheel), rrpm, FALLING);

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
}

void loop() {
  hz = 100/(rw_delt+1);
 // Serial.print("interupt took: ");
  //Serial.print(rw_delt);
 // Serial.println(" uS");
  
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
  
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.print(" | Z = "); Serial.println(AcZ); 
  
  Serial.print("Gyroscope: ");
  Serial.print("X = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.println(GyZ);
  Serial.println(" ");
  delay(333);
}

void rrpm(){
  unsigned long temp = micros();
  rw_delt = temp - tim;
  tim = temp;
  }


  
