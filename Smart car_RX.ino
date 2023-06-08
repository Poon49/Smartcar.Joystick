#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define CE_PIN 8
#define CSN_PIN 7

#define CHANNEL_READ   123456789

const byte R1 = 5;
const byte R2 = 3;
const byte L1 = 10;
const byte L2 = 6;
RF24 radio(CE_PIN, CSN_PIN);

int8_t data[5];

unsigned long packetsRead = 0;
unsigned long lastUpdate = 0;
int16_t packetsSec = 0;
unsigned long lastRecvTime = 0;
unsigned long drops = 0;
unsigned long lastPrint  = 0;
int16_t R=0, L=0, pl1=0, pl2=0, pr1=0, pr2=0;
const byte acc = 6;
const byte dcc = 20;
const byte minMotor = 95;
void setup()
{

  Serial.begin(9600);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  Serial.println("Nrf24L01 Receiver Starting");

  // Configure the Radio
  int failCount = 0;
  bool radioReady = false;
  while (!radioReady && failCount < 10) {
    radioReady = radio.begin();
    if (!radioReady) {
      failCount++;
      Serial.print("Radio Initialisation Failed ");
      Serial.print(failCount);
      Serial.println(" times.");
    }
  }
  if (!radioReady) {
    Serial.println("Radio could not be initialised!");
    return;
  }
  
  radio.openReadingPipe(1, CHANNEL_READ);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

}

void loop()
{
  while ( radio.available() ) {
    radio.read(&data, sizeof(data));
    packetsRead++;
    lastRecvTime = millis();
  }
  if ( millis() - lastUpdate > 1000 ) {
    packetsSec = packetsRead;
    packetsRead = 0;
    lastUpdate = millis();
  }
  if (millis() - lastPrint > 10000) {
    Serial.print("Pac: ");
    Serial.print(packetsSec);
    Serial.print("\tD0: ");
    Serial.print(data[0]);
    Serial.print("\td1: ");
    Serial.print(data[1]);
    Serial.print("\td2: ");
    Serial.print(data[2]);
    Serial.print("\td3: ");
    Serial.print(data[3]);
    Serial.print("\tD4: ");
    Serial.println(data[4]);
    lastPrint = millis();
  }
  
  if (packetsSec > 0) {   
      L=data[0]*2;
      R=data[1]*2;
      if(L!=0 &&L < minMotor && L>0) L = minMotor;
      else if(L!=0 &&L > -90 && L < 0 ) L = -minMotor;
      if(R!=0 &&R < minMotor && R > 0) R = minMotor;
      else if(R!=0 &&R > -minMotor && R< 0 ) R = -minMotor;
    } 
  else{
    R=0;
    L=0;
  }

  if(R>0){
      if(pr1>=0 && pr2 == 0){
        if(R>=pr1){
          if(pr1<minMotor)pr1=minMotor;
          pr1+=acc;
          if(pr1>R) pr1=R;
        }
        else{
          pr1-=8;
        }
      }
      else if(pr1 == 0 && pr2 > 0){
        pr2 -=dcc;
        if(pr2<0) {
          pr2 = 0; 
          analogWrite(R1,pr1);
          analogWrite(R2,pr2);
          delay(5);
          }  
      }
    }
      
      //---------------------------------------------
     else if(R<0){
      if(pr2>=0 && pr1 == 0){
        if(-R>=pr2){
          if(pr2<minMotor)pr2=minMotor;
          pr2+=acc;
          if(pr2>-R)pr2 = -R;
        }
        else{
          pr2-=8;
        }
      }
      else if(pr2 == 0 && pr1 > 0){
        pr1 -=dcc;
        if(pr1<0) {
          pr1 = 0; 
          analogWrite(R1,pr1);
          analogWrite(R2,pr2);
          delay(5);
        }
      }
      
     }
     //---------------------------------------------
     else{
      if(pr2 == 0 && pr1 > 0){
        pr1 -=dcc;
        if(pr1<0) {
          pr1 = 0; 
          analogWrite(R1,pr1);
          analogWrite(R2,pr2);
          delay(5);
        }       
      }
      else if(pr1 == 0 && pr2 > 0){
        pr2 -=dcc;
        if(pr2<0) {
          pr2 = 0; 
          analogWrite(R1,pr1);
          analogWrite(R2,pr2);
          delay(5);
        }    
      }
     }


     if(L>0){
      if(pl1>=0 && pl2 == 0){
        if(L>=pl1){
          if(pl1<minMotor)pl1=minMotor;
          pl1+=acc;
          if(pl1>L)pl1 = L;
        }
        else{
          pl1-=8;
        }
      }
      else if(pl1 == 0 && pl2 > 0){
        pl2 -=dcc;
        if(pl2<0) {
          pl2 = 0; 
          analogWrite(L1,pl1);
          analogWrite(L2,pl2);
          delay(5);
          }  
      }
    }
      
      //---------------------------------------------
     else if(L<0){
      if(pl2>=0 && pl1 == 0){
        if(-L>=pl2){
          if(pl2<minMotor)pl2=minMotor;
          pl2+=acc;
          if(pl2>-L)pl2 = -L;
        }
        else{
          pl2-=dcc;
        }
      }
      else if(pl2 == 0 && pl1 > 0){
        pl1 -=dcc;
        if(pl1<0) {
          pl1 = 0; 
          analogWrite(L1,pl1);
          analogWrite(L2,pl2);
          delay(5);
        }
      }
      
     }
     //---------------------------------------------
     else{
      if(pl2 == 0 && pl1 > 0){
        pl1 -=dcc;
        if(pl1<0) {
          pl1 = 0; 
          analogWrite(L1,pl1);
          analogWrite(L2,pl2);
          delay(5);
        }       
      }
      else if(pl1 == 0 && pl2 > 0){
        pl2 -=dcc;
        if(pl2<0) {
          pl2 = 0; 
          analogWrite(L1,pl1);
          analogWrite(L2,pl2);
          delay(5);
        }    
      }
     }
         
    pr1 = constrain(pr1,0,254);
    pr2 = constrain(pr2,0,254);
    pl1 = constrain(pl1,0,254);
    pl2 = constrain(pl2,0,254);
      
    analogWrite(R1,pr1);
    analogWrite(R2,pr2);
    analogWrite(L1,pl1);
    analogWrite(L2,pl2);
    
  delay(15);
  
}
void prints(){
    Serial.print(R);
    Serial.print("\t");
    Serial.print(pr1);
    Serial.print("\t");
    Serial.print(pr2);
    Serial.print("\t");
    Serial.print("\t");
    Serial.print(L);
    Serial.print("\t");
    Serial.print(pl1);
    Serial.print("\t");
    Serial.println(pl2);
}
