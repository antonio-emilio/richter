#ifndef KEY_MATRIX_H
#define KEY_MATRIX_H

#include <Arduino.h>

#define KEY_MATRIX_PHONE(name, r0, r1, r2, r3, c0, c1, c2) \
    char name##_keymap[4][3] = { \
    {'1', '2', '3'}, \
    {'4', '5', '6'}, \
    {'7', '8', '9'}, \
    {'*', '0', '#'}}; \
    byte name##_rowPins[4] = {r0, r1, r2, r3}; \
    byte name##_colPins[3] = {c0, c1, c2}; \
    KeyMatrixPhone name((char *) name##_keymap, (byte) 4, (byte) 3, name##_rowPins, name##_colPins)

// http://www.accordia.com.tw/products.html
// http://www.accordia.com.tw/spec.html
#define AK_304(name, p0, p1, p2, p3, p4, p5, p6) KEY_MATRIX_PHONE(name, p1, p6, p5, p3, p2, p0, p4);

#define KM_NONE 0
#define KM_KEYUP 1
#define KM_KEYDOWN 2
#define KM_TEXT 3
#define KM_MODE 4

#define KM_MODE_NONE -1
#define KM_MODE_LOWER 0
#define KM_MODE_UPPER 1
#define KM_MODE_NUM 2

struct KeyMatrixEvent {
    byte type;
    char c;
};

class KeyMatrix {

private:
    char *_keymap;
    byte _rows;
    byte _cols;
    byte *_rowPins;
    byte *_colPins;
    int _state;

public:
    KeyMatrixEvent event;

public:
    KeyMatrix(char *keymap, byte rows, byte cols, byte *rowPins, byte *colPins);
    int scanKeys();
    bool pollEvent();
    char getKey();

};

#include "KeyMatrixPhone.h"

#endif
