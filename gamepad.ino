#include <Arduino.h>
#include "HID-Project.h"

// ボタンとArduinoのピン番号の対応
#define PIN_FOR_BUTTON_1       2
#define PIN_FOR_BUTTON_2       3
#define PIN_FOR_BUTTON_3       4
#define PIN_FOR_BUTTON_4       5
#define PIN_FOR_BUTTON_5       6
#define PIN_FOR_BUTTON_6       7
#define PIN_FOR_BUTTON_7       8
#define PIN_FOR_BUTTON_10      15
#define PIN_FOR_BUTTON_11      10
#define PIN_FOR_BUTTON_12      14
#define PIN_FOR_BUTTON_13      9

// ボタンNo
#define BUTTON_1       1
#define BUTTON_2       2
#define BUTTON_3       3
#define BUTTON_4       4
#define BUTTON_5       5
#define BUTTON_6       6
#define BUTTON_7       7
#define BUTTON_8       8
#define BUTTON_9       9
#define BUTTON_10      10
#define BUTTON_11      11
#define BUTTON_12      12
#define BUTTON_13      13


//common
unsigned long elapsed = 0;

//scratch
const unsigned long ACTIVE_TIME = 40000; //us
const int THRESHOLD = 750;  // フォトインタラプタのON/OFFの閾値
const int THRES_LOW = 850;
const int THRES_HIGH = 650;
long scrTimer = 0;
bool toRight;
int d1f, d2f;
int list[8][4] = {
  {1, 1, 0, 1},
  {1, 0, 1, 1},
  {0, 0, 1, 0},
  {0, 1, 0, 0},
  {1, 1, 1, 0},
  {1, 0, 0, 0},
  {0, 1, 1, 1},
  {0, 0, 0, 1}
};

//keys
const int INTERVAL = 10;
unsigned long ptime[11];


void setup() {
  //common

  pinMode(PIN_FOR_BUTTON_1, INPUT_PULLUP);
  pinMode(PIN_FOR_BUTTON_2, INPUT_PULLUP);
  pinMode(PIN_FOR_BUTTON_3, INPUT_PULLUP);
  pinMode(PIN_FOR_BUTTON_4, INPUT_PULLUP);
  pinMode(PIN_FOR_BUTTON_5, INPUT_PULLUP);
  pinMode(PIN_FOR_BUTTON_6, INPUT_PULLUP);
  pinMode(PIN_FOR_BUTTON_7, INPUT_PULLUP);
  pinMode(PIN_FOR_BUTTON_10, INPUT_PULLUP);
  pinMode(PIN_FOR_BUTTON_11, INPUT_PULLUP);
  pinMode(PIN_FOR_BUTTON_12, INPUT_PULLUP);
  pinMode(PIN_FOR_BUTTON_13, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  elapsed = micros();

  //scratch
  toRight = false;
  d1f = analogRead(A0) < THRESHOLD ? 0 : 1;
  d2f = analogRead(A1) < THRESHOLD ? 0 : 1;

  //keys
  for (int i = 0; i < 11; i++)
    ptime[i] = 0;


  Gamepad.begin();
}

void loop() {
  //common
  unsigned long now = micros();
  if (now < elapsed) { //overflow!
    elapsed = now;
    return;
  }
  long tdif = now - elapsed;
  elapsed = now;

  //scratch
  int d1l, d2l;
  int thres1 = d1f == 0 ? THRES_LOW : THRES_HIGH;
  int thres2 = d2f == 0 ? THRES_LOW : THRES_HIGH;
  d1l = analogRead(A0) < thres1 ? 0 : 1;
  d2l = analogRead(A1) < thres2 ? 0 : 1;

  bool triggered = false;
  int state[4] = {d1f, d1l, d2f, d2l};
  for (int i = 0; i < 8; i++) {
    if (memcmp(list[i], state, sizeof(int) * 4) == 0) { //compare
      if (i < 4) {
        scrTimer = ACTIVE_TIME;
        toRight = true;
        triggered = true;
        break;
      } else {
        scrTimer = ACTIVE_TIME;
        toRight = false;
        triggered = true;
        break;
      }
    }
  }
  if (!triggered) {
    scrTimer -= tdif;
  }
  if (scrTimer > 0) {
    if (toRight) {
      //Right Turn
      Gamepad.press(BUTTON_8);
      Gamepad.release(BUTTON_9);
    } else {
      //Left Turn
      Gamepad.press(BUTTON_9);
      Gamepad.release(BUTTON_8);
    }
  } else {
    //No Input
    scrTimer = 0;
    Gamepad.release(BUTTON_8);
    Gamepad.release(BUTTON_9);


  }
  d1f = d1l;
  d2f = d2l;

  //keys
  ptime[0]++;
  if (!digitalRead(PIN_FOR_BUTTON_1)) {
    Gamepad.press(BUTTON_1);
    ptime[0] = 0;
  } else {
    if (ptime[0] < INTERVAL)return; //burst!
    Gamepad.release(BUTTON_1);
  }

  ptime[1]++;
  if (!digitalRead(PIN_FOR_BUTTON_2)) {
    Gamepad.press(BUTTON_2);
    ptime[1] = 0;
  } else {
    if (ptime[1] < INTERVAL)return; //burst!
    Gamepad.release(BUTTON_2);
  }

  ptime[2]++;
  if (!digitalRead(PIN_FOR_BUTTON_3)) {
    Gamepad.press(BUTTON_3);
    ptime[2] = 0;
  } else {
    if (ptime[2] < INTERVAL)return; //burst!
    Gamepad.release(BUTTON_3);
  }

  ptime[3]++;
  if (!digitalRead(PIN_FOR_BUTTON_4)) {
    Gamepad.press(BUTTON_4);
    ptime[3] = 0;
  } else {
    if (ptime[3] < INTERVAL)return; //burst!
    Gamepad.release(BUTTON_4);
  }

  ptime[4]++;
  if (!digitalRead(PIN_FOR_BUTTON_5)) {
    Gamepad.press(BUTTON_5);
    ptime[4] = 0;
  } else {
    if (ptime[4] < INTERVAL)return; //burst!
    Gamepad.release(BUTTON_5);
  }

  ptime[5]++;
  if (!digitalRead(PIN_FOR_BUTTON_6)) {
    Gamepad.press(BUTTON_6);
    ptime[5] = 0;
  } else {
    if (ptime[5] < INTERVAL)return; //burst!
    Gamepad.release(BUTTON_6);
  }

  ptime[6]++;
  if (!digitalRead(PIN_FOR_BUTTON_7)) {
    Gamepad.press(BUTTON_7);
    ptime[6] = 0;
  } else {
    if (ptime[6] < INTERVAL)return; //burst!
    Gamepad.release(BUTTON_7);
  }

  ptime[7]++;
  if (!digitalRead(PIN_FOR_BUTTON_10)) {
    Gamepad.press(BUTTON_10);
    ptime[7] = 0;
  } else {
    if (ptime[7] < INTERVAL)return; //burst!
    Gamepad.release(BUTTON_10);
  }

  ptime[8]++;
  if (!digitalRead(PIN_FOR_BUTTON_11)) {
    Gamepad.press(BUTTON_11);
    ptime[8] = 0;
  } else {
    if (ptime[8] < INTERVAL)return; //burst!
    Gamepad.release(BUTTON_11);
  }

  ptime[9]++;
  if (!digitalRead(PIN_FOR_BUTTON_12)) {
    Gamepad.press(BUTTON_12);
    ptime[9] = 0;
  } else {
    if (ptime[9] < INTERVAL)return; //burst!
    Gamepad.release(BUTTON_12);
  }

  ptime[10]++;
  if (!digitalRead(PIN_FOR_BUTTON_13)) {
    Gamepad.press(BUTTON_13);
    ptime[10] = 0;
  } else {
    if (ptime[10] < INTERVAL)return; //burst!
    Gamepad.release(BUTTON_13);
  }


  Gamepad.write();
}


