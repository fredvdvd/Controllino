#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

/*
  CONTROLLINO - Blinking_LEDs, Version 01.00

  Turns on and off digital outputs of CONTROLLINO, which also blinks with CONTROLLINO´s status LEDs for the outputs.

  IMPORTANT INFORMATION!
  Please, select proper target board in Tools->Board->Controllino MINI/MAXI/MEGA before Upload to your CONTROLLINO.
  (Please, refer to https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library if you do not see the CONTROLLINOs in the Arduino IDE menu Tools->Board.)

  This example sketch does not require any additional parts, equipment, wires or power supply. Just your CONTROLLINO, USB cable (type A-B) and a PC.
 
  Created 8 Dec 2016
  by Lukas

  https://controllino.biz/

  (Check https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library for the latest CONTROLLINO related software stuff.)
*/
byte AInputs[] = {
  CONTROLLINO_A0, CONTROLLINO_A1, CONTROLLINO_A2, CONTROLLINO_A3, CONTROLLINO_A4, 
  CONTROLLINO_A5, CONTROLLINO_A6, CONTROLLINO_A7, CONTROLLINO_A8, CONTROLLINO_A9,
  CONTROLLINO_A10, CONTROLLINO_A11, CONTROLLINO_A12, CONTROLLINO_A13};
static const uint8_t RPorts[] = {
  CONTROLLINO_R0, CONTROLLINO_R1, CONTROLLINO_R2, CONTROLLINO_R3, CONTROLLINO_R4,
  CONTROLLINO_R5, CONTROLLINO_R6, CONTROLLINO_R7, CONTROLLINO_R8, CONTROLLINO_R9};
boolean AIReading[] =    { LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW };
boolean AIState[] =      { LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW };
boolean AILastState[] =  { LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW };
boolean RPortsState[] = { LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW };
unsigned long lastDebounceTime[] = { 0,0,0,0,0,0,0,0,0 }; 
unsigned long debounceDelay = 50; 



void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 9; i++) {
    pinMode(AInputs[i], INPUT); 
  }
  for (int i = 0; i < 10; i++) {
    pinMode(RPorts[i], OUTPUT); 
  }
}

void loop() {
  for (int i = 0; i < 9; i++) {
    AIReading[i] = digitalRead(AInputs[i]);
    if (AIReading[i] != AILastState[i]) {  
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (AIReading[i] != AIState[i]) {
        AIState[i] = AIReading[i];
        if (AIState[i] == HIGH) {
        RPortsState[i] = !RPortsState[i];
        }
      }
    }
  digitalWrite(RPorts[i], RPortsState[i]);
  AILastState[i] = AIReading[i];
  }
}
//digitalWrite(RPorts[0], LOW);
//  delay(10000);  
//digitalWrite(RPorts[0], HIGH);   
//  delay(10000);
