#include <KeyMatrix.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

// create buffer for the text
#define BUFF_LEN 50
char buff[BUFF_LEN];

// create oled
Adafruit_SSD1306 oled(0);
#define OLED_FNT_W 6
#define OLED_FNT_H 8

const char *mode_str[] = {"abc", "ABC", "123"};

void oled_draw_text(String str) {
  oled.fillRect(0, OLED_FNT_H, SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT, BLACK);
  oled.setCursor(0, OLED_FNT_H);
  oled.println(str);
  oled.display();
}

void toogle_txt_mode() {
  if (keypad.mode() == KM_MODE_NONE) {
    // put keypad in text mode
    keypad.startTextMode(buff, BUFF_LEN);
    Serial.println("(TEXT MODE ON)");
    // clear and draw header
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.print(mode_str[keypad.mode()]);
    oled.print("  *: BKSP #: MODE");
    oled.display();
  } else {
    // exit text mode
    keypad.stopTextMode();
    Serial.println("(TEXT MODE OFF)");
    // clear and draw header
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.print("0: TEXT MODE");
    oled.display();
  }
}

void setup() {
  Serial.begin(9600);

  // initialize oled
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);
  oled.setTextSize(1);
  oled.setTextColor(WHITE, BLACK);

  toogle_txt_mode();
}

void loop() {
  // poll events from the keypad
  if (keypad.pollEvent()) {
    String ev_str = event_to_string(keypad.event);

    // print to serial
    Serial.println(ev_str);

    // handle text and mode
    if (keypad.event.type == KM_TEXT) {
      // text change event
      Serial.println(buff);
      oled_draw_text(buff);
    } else if (keypad.event.type == KM_MODE) {
      // mode change event (abc, ABC, 123)
      oled.setCursor(0, 0);
      oled.print(mode_str[keypad.event.c]);
      oled.display();
    } else if (keypad.event.type == KM_KEYDOWN && keypad.event.c == '0') {
      toogle_txt_mode();
    } else if (keypad.mode() == KM_MODE_NONE) {
      oled_draw_text(ev_str);
    }
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
