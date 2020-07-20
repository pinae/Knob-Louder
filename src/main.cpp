#include <Arduino.h>
#include <HID-Project.h>

#define ENCODER_A_PIN 3
#define ENCODER_B_PIN 2
#define SWITCH_PIN 4
#define STATE_LOCKED 0
#define STATE_TURN_RIGHT_START 1
#define STATE_TURN_RIGHT_MIDDLE 2
#define STATE_TURN_RIGHT_END 3
#define STATE_TURN_LEFT_START 4
#define STATE_TURN_LEFT_MIDDLE 5
#define STATE_TURN_LEFT_END 6
#define STATE_UNDECIDED 7

uint8_t encoderState = 0;
uint8_t switchState = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT);
  encoderState = 0;
  switchState = 0;
  Consumer.begin();
}

void stepRight() {
  Serial.println(" -->");
  Consumer.write(MEDIA_VOL_UP);
}

void stepLeft() {
  Serial.println("<-- ");
  Consumer.write(MEDIA_VOL_DOWN);
}

void switchPress() {
  Serial.println("- SWITCH -");
  Consumer.write(MEDIA_VOL_MUTE);
}

void loop() {
  uint8_t a = digitalRead(ENCODER_A_PIN);
  uint8_t b = digitalRead(ENCODER_B_PIN);
  uint8_t s = digitalRead(SWITCH_PIN);
  switch (encoderState) {
    case STATE_LOCKED: 
      if (a && b)       { encoderState = STATE_UNDECIDED; }
      else if (!a && b) { encoderState = STATE_TURN_LEFT_START; }
      else if (a && !b) { encoderState = STATE_TURN_RIGHT_START; }
      else              { encoderState = STATE_LOCKED; }; 
      break;
    case STATE_TURN_RIGHT_START: 
      if (a && b)       { encoderState = STATE_TURN_RIGHT_MIDDLE; }
      else if (!a && b) { encoderState = STATE_TURN_RIGHT_END; }
      else if (a && !b) { encoderState = STATE_TURN_RIGHT_START; }
      else              { encoderState = STATE_LOCKED; }; 
      break;
    case STATE_TURN_RIGHT_MIDDLE:
    case STATE_TURN_RIGHT_END:
      if (a && b)       { encoderState = STATE_TURN_RIGHT_MIDDLE; }
      else if (!a && b) { encoderState = STATE_TURN_RIGHT_END; }
      else if (a && !b) { encoderState = STATE_TURN_RIGHT_START; }
      else              { encoderState = STATE_LOCKED; stepRight(); }; 
      break;
    case STATE_TURN_LEFT_START: 
      if (a && b)       { encoderState = STATE_TURN_LEFT_MIDDLE; }
      else if (!a && b) { encoderState = STATE_TURN_LEFT_START; }
      else if (a && !b) { encoderState = STATE_TURN_LEFT_END; }
      else              { encoderState = STATE_LOCKED; }; 
      break;
    case STATE_TURN_LEFT_MIDDLE: 
    case STATE_TURN_LEFT_END: 
      if (a && b)       { encoderState = STATE_TURN_LEFT_MIDDLE; }
      else if (!a && b) { encoderState = STATE_TURN_LEFT_START; }
      else if (a && !b) { encoderState = STATE_TURN_LEFT_END; }
      else              { encoderState = STATE_LOCKED; stepLeft(); }; 
      break;
    case STATE_UNDECIDED:
      if (a && b)       { encoderState = STATE_UNDECIDED; }
      else if (!a && b) { encoderState = STATE_TURN_RIGHT_END; }
      else if (a && !b) { encoderState = STATE_TURN_LEFT_END; }
      else              { encoderState = STATE_LOCKED; }; 
      break;
  }
  if (!switchState && s) switchPress();
  switchState = s;
}