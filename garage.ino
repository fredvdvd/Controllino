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



#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include <Ethernet.h>
#include <ArduinoHA.h>


#define BROKER_ADDR IPAddress(192,168,1,118)

byte mac[] = {0x00, 0x10, 0xFA, 0x6E, 0x38, 0x4A};

EthernetClient client;
HADevice device(mac, sizeof(mac));
HAMqtt mqtt(client, device);

#define NUM_SWITCHES 10
HASwitch switch0("mySwitch0");
HASwitch switch1("mySwitch1");
HASwitch switch2("mySwitch2");
HASwitch switch3("mySwitch3");
HASwitch switch4("mySwitch4");
HASwitch switch5("mySwitch5");
HASwitch switch6("mySwitch6");
HASwitch switch7("mySwitch7");
HASwitch switch8("mySwitch8");
HASwitch switch9("mySwitch9");

byte AInputs[] = {
  CONTROLLINO_A0, CONTROLLINO_A1, CONTROLLINO_A2, CONTROLLINO_A3, CONTROLLINO_A4,
  CONTROLLINO_A5, CONTROLLINO_A6, CONTROLLINO_A7, CONTROLLINO_A8, CONTROLLINO_A9
};
static const uint8_t RPorts[] = {
  CONTROLLINO_R0, CONTROLLINO_R1, CONTROLLINO_R2, CONTROLLINO_R3, CONTROLLINO_R4,
  CONTROLLINO_R5, CONTROLLINO_R6, CONTROLLINO_R7, CONTROLLINO_R8, CONTROLLINO_R9
};
boolean AIReading[] =    { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
boolean AIState[] =      { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
boolean AILastState[] =  { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
boolean RPortsState[] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
unsigned long lastDebounceTime[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned long debounceDelay = 50;


void onSwitchCommand(bool state, HASwitch* sender )
{
  boolean portState = state ? LOW : HIGH; // LOW means OFF, HIGH means ON

    if (sender == &switch0) {
      digitalWrite(RPorts[0], portState);
    } else if (sender == &switch1) {
      digitalWrite(RPorts[1], portState);
    } else if (sender == &switch2) {
      digitalWrite(RPorts[2], portState);
    } else if (sender == &switch3) {
      digitalWrite(RPorts[3], portState);
    } else if (sender == &switch4) {
      digitalWrite(RPorts[4], portState);
    } else if (sender == &switch5) {
      digitalWrite(RPorts[5], portState);
    } else if (sender == &switch6) {
      digitalWrite(RPorts[6], portState);
    } else if (sender == &switch7) {
      digitalWrite(RPorts[7], portState);
    } else if (sender == &switch8) {
      digitalWrite(RPorts[8], portState);
    } else if (sender == &switch9) {
      digitalWrite(RPorts[9], portState);
    }
  }

  sender->setState(state); // report state back to the Home Assistant
}


void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 9; i++) {
    pinMode(AInputs[i], INPUT);
  }
  for (int i = 0; i < 10; i++) {
    pinMode(RPorts[i], OUTPUT);
  }

  Ethernet.begin(mac);

  switch0.setName("Pretty label 0");
  switch0.setIcon("mdi:lightbulb");
  switch0.onCommand(onSwitchCommand);

  switch1.setName("Pretty label 1");
  switch1.setIcon("mdi:lightbulb");
  switch1.onCommand(onSwitchCommand);

  switch2.setName("Pretty label 2");
  switch2.setIcon("mdi:lightbulb");
  switch2.onCommand(onSwitchCommand);

  switch3.setName("Pretty label 3");
  switch3.setIcon("mdi:lightbulb");
  switch3.onCommand(onSwitchCommand);
  switch4.setName("Pretty label 4");
  switch4.setIcon("mdi:lightbulb");
  switch4.onCommand(onSwitchCommand);
  switch5.setName("Pretty label 5");
  switch5.setIcon("mdi:lightbulb");
  switch5.onCommand(onSwitchCommand);
  switch6.setName("Pretty label 6");
  switch6.setIcon("mdi:lightbulb");
  switch6.onCommand(onSwitchCommand);
  switch7.setName("Pretty label 7");
  switch7.setIcon("mdi:lightbulb");
  switch7.onCommand(onSwitchCommand);
  switch8.setName("Pretty label 8");
  switch8.setIcon("mdi:lightbulb");
  switch8.onCommand(onSwitchCommand);
  switch9.setName("Pretty label 9");
  switch9.setIcon("mdi:lightbulb");
  switch9.onCommand(onSwitchCommand);

  mqtt.begin(BROKER_ADDR);
}

void loop() {
  Ethernet.maintain();
  mqtt.loop();

  for (int i = 0; i < 10; i++) {
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
    switch(i) {
      case 0: switch0.setState(AIState[i]); break;
      case 1: switch1.setState(AIState[i]); break;
      case 2: switch2.setState(AIState[i]); break;
      case 3: switch3.setState(AIState[i]); break;
      case 4: switch4.setState(AIState[i]); break;
      case 5: switch5.setState(AIState[i]); break;
      case 6: switch6.setState(AIState[i]); break;
      case 7: switch7.setState(AIState[i]); break;
      case 8: switch8.setState(AIState[i]); break;
      case 9: switch9.setState(AIState[i]); break;
    }
    AILastState[i] = AIReading[i];
  }
}

