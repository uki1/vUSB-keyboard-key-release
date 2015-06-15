#include "UsbKeyboard.h"

const int data1 = A2;
const int latch1 = A1;
const int clock1 = A0;

const int data2 = A4;
const int latch2 = A3;
const int clock2 = A5;
const int TICK = 6;

unsigned long key_state1[8] = {
  0,0,0,0,0,0,0,0};
int KEY_NUM_1[8] = {
  28,//0-A 
  29,//1-B
  21,//2-sel
  5,//3-start
  82,//4-up
  81,//5-down
  80,//6-left
  79//7-right
};
unsigned long key_state2[8] = {
  0,0,0,0,0,0,0,0};//
int KEY_NUM_2[8] = {
  8,//0-A 
  9,//1-B
  11,//2-sel
  15,//3-start
  26,//4-up
  16,//5-down
  4,//6-left
  7//7-right
};

void init_joystick(int data, int latch, int clock) {
  pinMode(data, INPUT);
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);
  digitalWrite(clock, HIGH);
}

void get_keys_state_joystick1(int data, int latch, int clock) {
  digitalWrite(latch, HIGH);
  delayMicroseconds(TICK);
  digitalWrite(latch, LOW);
  for (int i = 0; i < 8; ++i) {
    delayMicroseconds(TICK);
    digitalWrite(clock, LOW);
    int ns = digitalRead(data);

    if(!ns) {
      key_state1[i] = millis();
    }

    if(ns && (key_state1[i] + 1000) > millis()) {
      key_state1[i] = 0;
    }
    
    delayMicroseconds(TICK);
    digitalWrite(clock, HIGH);
  }
  delayMicroseconds(TICK);
}

void get_keys_state_joystick2(int data, int latch, int clock) {
  digitalWrite(latch, HIGH);
  delayMicroseconds(TICK);
  digitalWrite(latch, LOW);
  for (int i = 0; i < 8; ++i) {
    delayMicroseconds(TICK);
    digitalWrite(clock, LOW);
    int ns = digitalRead(data);
    if(!ns) {
      key_state2[i] = millis();
    }

    if(ns && (key_state2[i] + 1000) > millis()) {
      key_state2[i] = 0;

    }
    delayMicroseconds(TICK);
    digitalWrite(clock, HIGH);
  }
  delayMicroseconds(TICK);
}

void setup() {
  init_joystick(data1, latch1, clock1);
  delay(1);
  init_joystick(data2, latch2, clock2);
  UsbKeyboard.update();
  //Serial.begin(9600);
  delay(200);
}

void loop() {
  UsbKeyboard.update();
  init_joystick(data2, latch2, clock2);
  init_joystick(data1, latch1, clock1);//это возможно решает проблему с залипанием
  
  get_keys_state_joystick1(data1, latch1, clock1);//get  key_state1[i]
  delayMicroseconds(TICK);
  get_keys_state_joystick2(data2, latch2, clock2);
  
  for (int i = 0; i < 8; ++i){
    if (key_state1[i] > 0 & UsbKeyboard.KeyIsDown(KEY_NUM_1[i]) == 0){
      UsbKeyboard.KeyD(KEY_NUM_1[i]);
    }
    if (key_state1[i] == 0 & UsbKeyboard.KeyIsDown(KEY_NUM_1[i]) == 1){
      UsbKeyboard.KeyU(KEY_NUM_1[i]);
    }
  }
  
  for (int i = 0; i < 8; ++i){
    if (key_state2[i] > 0 & UsbKeyboard.KeyIsDown(KEY_NUM_2[i]) == 0){
      UsbKeyboard.KeyD(KEY_NUM_2[i]);
    }
    if (key_state2[i] == 0 & UsbKeyboard.KeyIsDown(KEY_NUM_2[i]) == 1){
      UsbKeyboard.KeyU(KEY_NUM_2[i]);
      
    }
  }
}


