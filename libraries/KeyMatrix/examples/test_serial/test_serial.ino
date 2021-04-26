#include <KeyMatrix.h>

// create keypad
// AK-304 is a common phone keypad with alpha-numeric legend
// http://www.accordia.com.tw/spec.html
// arduino pins from left to right on the keypad

AK_304(keypad, 4, 5, 6, 7, 8, 9, 10);

// for other phone keypads use
// KEY_MATRIX_PHONE(keypad, r0, r1, r2, r3, c0, c1, c2)
// with row and column pins

// and for other generic keypads use
// KeyMatrix keypad(keymap, rows, cols, rowPins, colPins);
// see KeyMatrix.h for help

void setup() {
  Serial.begin(9600);
}

void loop() {
  // poll events from the keypad
  if (keypad.pollEvent()) {
    // print to serial
    Serial.println(event_to_string(keypad.event));
  }
}

// helper function to print events

String event_to_string(const KeyMatrixEvent &ev) {
  String s;
  switch (ev.type) {
    case KM_NONE: s = "KM_NONE ("; break;
    case KM_KEYUP: s = "KM_KEYUP ("; break;
    case KM_KEYDOWN: s = "KM_KEYDOWN ("; break;
    case KM_TEXT: s = "KM_TEXT ("; break;
    case KM_MODE: s = "KM_MODE ("; break;
  }
  if (ev.type == KM_MODE) {
    switch (ev.c) {
      case KM_MODE_LOWER: s += "KM_MODE_LOWER"; break;
      case KM_MODE_UPPER: s += "KM_MODE_UPPER"; break;
      case KM_MODE_NUM: s += "KM_MODE_NUM"; break;
    }
  } else {
    s += ev.c;
  }
  s += ")";
  return s;
}
