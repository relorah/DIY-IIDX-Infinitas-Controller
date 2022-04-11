#include <Arduino.h>
#include "HID-Project.h"

// ボタンとArduinoのピン番号の対応
#define KEY_1       2
#define KEY_2       3
#define KEY_3       4
#define KEY_4       5
#define KEY_5       6
#define KEY_6       7
#define KEY_7       8
#define START       9
#define SELECT      10
#define BACKSPACE   14
#define ESC         15

// ボタンとキーボードのキーの対応
#define KEY_CODE_1          'z'
#define KEY_CODE_2          's'
#define KEY_CODE_3          'x'
#define KEY_CODE_4          'd'
#define KEY_CODE_5          'c'
#define KEY_CODE_6          'f'
#define KEY_CODE_7          'v'
#define KEY_CODE_START      KEY_RETURN
#define KEY_CODE_SELECT     KEY_TAB
#define KEY_CODE_BACKSPACE  KEY_BACKSPACE
#define KEY_CODE_ESC        KEY_ESC 
#define KEY_CODE_SCR1       KEY_LEFT_SHIFT
#define KEY_CODE_SCR2       KEY_LEFT_CTRL

//common
unsigned long elapsed = 0;

//scratch
const unsigned long ACTIVE_TIME = 40000; //スクラッチの入力時間
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
  NKROKeyboard.begin();
  pinMode(KEY_1, INPUT_PULLUP);
  pinMode(KEY_2, INPUT_PULLUP);
  pinMode(KEY_3, INPUT_PULLUP);
  pinMode(KEY_4, INPUT_PULLUP);
  pinMode(KEY_5, INPUT_PULLUP);
  pinMode(KEY_6, INPUT_PULLUP);
  pinMode(KEY_7, INPUT_PULLUP);
  pinMode(START, INPUT_PULLUP);
  pinMode(SELECT, INPUT_PULLUP);
  pinMode(BACKSPACE, INPUT_PULLUP);
  pinMode(ESC, INPUT_PULLUP);
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
      NKROKeyboard.add(KEY_CODE_SCR1);
      NKROKeyboard.remove(KEY_CODE_SCR2);
    } else {
      //Left Turn
      NKROKeyboard.add(KEY_CODE_SCR2);
      NKROKeyboard.remove(KEY_CODE_SCR1);
    }
  } else {
    //No Input
    scrTimer = 0;
    NKROKeyboard.remove(KEY_CODE_SCR1);
    NKROKeyboard.remove(KEY_CODE_SCR2);
  }
  d1f = d1l;
  d2f = d2l;

  //keys
  ptime[0]++;
  if (!digitalRead(KEY_1)) {
    NKROKeyboard.add(KEY_CODE_1);
    ptime[0] = 0;
  } else {
    if (ptime[0] < INTERVAL)return; //burst!
    NKROKeyboard.remove(KEY_CODE_1);
  }

  ptime[1]++;
  if (!digitalRead(KEY_2)) {
    NKROKeyboard.add(KEY_CODE_2);
    ptime[1] = 0;
  } else {
    if (ptime[1] < INTERVAL)return; //burst!
    NKROKeyboard.remove(KEY_CODE_2);
  }

  ptime[2]++;
  if (!digitalRead(KEY_3)) {
    NKROKeyboard.add(KEY_CODE_3);
    ptime[2] = 0;
  } else {
    if (ptime[2] < INTERVAL)return; //burst!
    NKROKeyboard.remove(KEY_CODE_3);
  }

  ptime[3]++;
  if (!digitalRead(KEY_4)) {
    NKROKeyboard.add(KEY_CODE_4);
    ptime[3] = 0;
  } else {
    if (ptime[3] < INTERVAL)return; //burst!
    NKROKeyboard.remove(KEY_CODE_4);
  }

  ptime[4]++;
  if (!digitalRead(KEY_5)) {
    NKROKeyboard.add(KEY_CODE_5);
    ptime[4] = 0;
  } else {
    if (ptime[4] < INTERVAL)return; //burst!
    NKROKeyboard.remove(KEY_CODE_5);
  }

  ptime[5]++;
  if (!digitalRead(KEY_6)) {
    NKROKeyboard.add(KEY_CODE_6);
    ptime[5] = 0;
  } else {
    if (ptime[5] < INTERVAL)return; //burst!
    NKROKeyboard.remove(KEY_CODE_6);
  }

  ptime[6]++;
  if (!digitalRead(KEY_7)) {
    NKROKeyboard.add(KEY_CODE_7);
    ptime[6] = 0;
  } else {
    if (ptime[6] < INTERVAL)return; //burst!
    NKROKeyboard.remove(KEY_CODE_7);
  }

  ptime[7]++;
  if (!digitalRead(START)) {
    NKROKeyboard.add(KEY_CODE_START);
    ptime[7] = 0;
  } else {
    if (ptime[7] < INTERVAL)return; //burst!
    NKROKeyboard.remove(KEY_CODE_START);
  }

  ptime[8]++;
  if (!digitalRead(SELECT)) {
    NKROKeyboard.add(KEY_CODE_SELECT);
    ptime[8] = 0;
  } else {
    if (ptime[8] < INTERVAL)return; //burst!
    NKROKeyboard.remove(KEY_CODE_SELECT);
  }

  ptime[9]++;
  if (!digitalRead(BACKSPACE)) {
    NKROKeyboard.add(KEY_CODE_BACKSPACE);
    ptime[9] = 0;
  } else {
    if (ptime[9] < INTERVAL)return; //burst!
    NKROKeyboard.remove(KEY_CODE_BACKSPACE);
  }

  ptime[10]++;
  if (!digitalRead(ESC)) {
    NKROKeyboard.add(KEY_CODE_ESC);
    ptime[10] = 0;
  } else {
    if (ptime[10] < INTERVAL)return; //burst!
    NKROKeyboard.remove(KEY_CODE_ESC);
  }


  NKROKeyboard.send();
}
