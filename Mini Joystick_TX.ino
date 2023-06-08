#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define CE_PIN  10
#define CSN_PIN 9

#define CHANNEL_WRITE   123456789

const byte poten_Pin = A3;
const byte moveY_Pin = A1;
const byte moveX_Pin = A2;
const byte JOY_SW = 2;
const byte Button_SW = 3;

int moveY = 0;
int moveX = 0;
int poten = 0;
int P = 0;
int X = 0;
int Y = 0;
bool A;
bool B;
unsigned long lastTimeSent = 0;
int8_t data[5];

RF24 radio(CE_PIN, CSN_PIN); 


void setup() {

  Serial.begin(9600);
  pinMode(JOY_SW, INPUT_PULLUP);
  radio.begin();
  radio.openWritingPipe(CHANNEL_WRITE);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

}

void loop() {
  
  moveY = analogRead(moveY_Pin);
  moveX = analogRead(moveX_Pin);
  poten = analogRead(poten_Pin);
  A= digitalRead(JOY_SW);
  B= digitalRead(Button_SW);
  X= map(moveX, 0, 1023, -126, 126);
  Y= map(moveY, 0, 1023, -126, 126);
  P= map(poten, 0, 1023, -126, 126);
  uint8_t trims = map(poten, 0, 1023, -20, 20);
  if((X<30)&&X>-30) X = 0;
  if((Y<30)&&Y>-30) Y = 0;
  data[0]= constrain((Y+(X/4))*(-1),-80, 80);
  data[1]= constrain((Y-(X/4))*(-1),-80, 80);
  if(data[0]!=0){
    data[0]+=trims;
  }
  if(data[1]!=0){
    data[1]-=trims;
  }
  data[0]= constrain(data[0],-80, 80);
  data[1]= constrain(data[1],-80, 80);
  data[2] = P;
  data[3] = B;
  data[4] = A;
  Serial.print("X: ");
  Serial.print(X);
  Serial.print("\t\tY: ");
  Serial.print(Y);
  Serial.print("\tData[0]: ");
  Serial.print(data[0]);
  Serial.print("\tData[1]: ");
  Serial.print(data[1]);
  Serial.print("\tData[2]: ");
  Serial.print(data[2]);
  Serial.print("\tData[3]: ");
  Serial.print(data[3]);
  Serial.print("\tData[4]: ");
  Serial.println(data[4]);
  radio.write(data, sizeof(data));
  delay(30);
}
