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

#define BROKER_USERNAME "yyyyyyy"
#define BROKER_PASSWORD "xxxxxxxxxx"
byte mac[] = { 0x00, 0x10, 0xFA, 0x6E, 0x38, 0x4A };

EthernetClient client;
HADevice device(mac, sizeof(mac));
HAMqtt mqtt(client, device);

HASwitch switch0("mySwitch0");
HASwitch switch1("mySwitch1");
HASwitch switch2("mySwitch2");
// HASwitch switch3("mySwitch3");
HASwitch switch4("mySwitch4");
HASwitch switch5("mySwitch5");
// HASwitch switch6("mySwitch6");
// HASwitch switch7("mySwitch7");
// HASwitch switch8("mySwitch8");
HASwitch switch9("mySwitch9");

HASwitch switches[] = {
  switch0, switch1, switch2, switch4, switch5, switch9
  // switch6, switch7, switch8, switch9, switch3
};

byte AInputs[] = {
  CONTROLLINO_A0, CONTROLLINO_A1, CONTROLLINO_A2, CONTROLLINO_A3, CONTROLLINO_A4,
  CONTROLLINO_A5, CONTROLLINO_A6, CONTROLLINO_A7, CONTROLLINO_A8, CONTROLLINO_A9
};
static const uint8_t RPorts[] = {
  CONTROLLINO_R0, CONTROLLINO_R1, CONTROLLINO_R2, CONTROLLINO_R3, CONTROLLINO_R4,
  CONTROLLINO_R5, CONTROLLINO_R6, CONTROLLINO_R7, CONTROLLINO_R8, CONTROLLINO_R9
};
boolean AIReading[] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
boolean AIState[] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
boolean AILastState[] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
boolean RPortsState[] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
unsigned long lastDebounceTime[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 9; i++) {
    pinMode(AInputs[i], INPUT);
  }
  for (int i = 0; i < 9; i++) {
    pinMode(RPorts[i], OUTPUT);
  }

  Ethernet.begin(mac);
  switch1.setName("Ingang garage");
  switch1.setIcon("mdi:lightbulb");
  switch1.onCommand(onSwitchCommand);

  switch2.setName("Werkbank");
  switch2.setIcon("mdi:lightbulb");
  switch2.onCommand(onSwitchCommand);

  switch4.setName("Zolder");
  switch4.setIcon("mdi:lightbulb");
  switch4.onCommand(onSwitchCommand);

  switch5.setName("Butshok");
  switch5.setIcon("mdi:lightbulb");
  switch5.onCommand(onSwitchCommand);

  switch9.setName("Buitenverlichting tuin");
  switch9.setIcon("mdi:lightbulb");
  switch9.onCommand(onSwitchCommand);

  switch0.setName("label 0");
  switch0.setIcon("mdi:lightbulb");
  switch0.onCommand(onSwitchCommand);

  mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
  //  delay(2000);



  // switch3.setName("label 3");
  // switch3.setIcon("mdi:lightbulb");
  // switch3.onCommand(onSwitchCommand);

  // switch6.setName("X6");
  // switch6.setIcon("mdi:lightbulb");
  // switch6.onCommand(onSwitchCommand);

  // switch7.setName("X7");
  // switch7.setIcon("mdi:lightbulb");
  // switch7.onCommand(onSwitchCommand);
  // mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
}

void onSwitchCommand(bool state, HASwitch* sender) {
  boolean portState = LOW;

  if (state == true) {
    portState = HIGH;
  }

  if (sender == &switch1) {
    RPortsState[1] = portState;
    digitalWrite(RPorts[1], RPortsState[1]);
  } else if (sender == &switch2) {
    RPortsState[2] = portState;
    digitalWrite(RPorts[2], RPortsState[2]);
  // } else if (sender == &switch3) {
  //   RPortsState[3] = portState;
  //   digitalWrite(RPorts[3], RPortsState[3]);
  } else if (sender == &switch4) {
    RPortsState[4] = portState;
    digitalWrite(RPorts[4], RPortsState[4]);
  } else if (sender == &switch5) {
    RPortsState[5] = portState;
    digitalWrite(RPorts[5], RPortsState[5]);
   } else if (sender == &switch9) {
     RPortsState[9] = portState;
     digitalWrite(RPorts[9], RPortsState[9]);
  } else {
  }

  sender->setState(state);  // report state back to the Home Assistant
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
      case 0: switch0.setState(RPortsState[i]); break;
      case 1: switch1.setState(RPortsState[i]); break;
      case 2: switch2.setState(RPortsState[i]); break;
      //case 3: switch3.setState(AIState[i]); break;
      case 4: switch4.setState(RPortsState[i]); break;
      case 5: switch5.setState(RPortsState[i]); break;
      //case 6: switch6.setState(AIState[i]); break;
      //case 7: switch7.setState(AIState[i]); break;
      //case 8: switch8.setState(AIState[i]); break;
      case 9: switch9.setState(AIState[i]); break;
    }
    AILastState[i] = AIReading[i];
  }
}
