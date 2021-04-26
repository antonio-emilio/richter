#include "KeyMatrix.h"

KeyMatrix::KeyMatrix(char *keymap, byte rows, byte cols, byte *rowPins, byte *colPins) :
_keymap(keymap),
_rows(rows),
_cols(cols),
_rowPins(rowPins),
_colPins(colPins),
_state(0) { }

int KeyMatrix::scanKeys() {
    int state = 0;
    for (int r = 0; r < _rows; r++) {
        pinMode(_rowPins[r], INPUT_PULLUP);
    }
    for (int c = _cols - 1; c >= 0; c--) {
        pinMode(_colPins[c], OUTPUT);
        digitalWrite(_colPins[c], LOW);
        for (int r = _rows - 1; r >= 0; r--) {
            state = (state << 1) | (!digitalRead(_rowPins[r]) & 1);
        }
        pinMode(_colPins[c], INPUT);
    }
    return state;
}

bool KeyMatrix::pollEvent() {
    int state = scanKeys();
    int lastState = _state;
    int i = 0;
    for (int c = 0; c < _cols; c++) {
        for (int r = 0; r < _rows; r++, i++) {
            if (state & 1 && !(lastState & 1)) {
                event.type = KM_KEYDOWN;
                event.c = _keymap[c + r*_cols];
                _state |= 1 << i;
                return true;
            } else if (!(state & 1) && lastState & 1) {
                event.type = KM_KEYUP;
                event.c = _keymap[c + r*_cols];
                _state &= ~(1 << i);
                return true;
            }
            state = state >> 1;
            lastState = lastState >> 1;
        }
    }
    event.type = KM_NONE;
    event.c = 0;
    return false;
}

char KeyMatrix::getKey() {
    if (pollEvent() && event.type == KM_KEYDOWN) {
        return event.c;
    }
    return 0;
}
