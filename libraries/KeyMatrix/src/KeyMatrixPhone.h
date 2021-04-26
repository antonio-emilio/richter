#ifndef KEY_MATRIX_PHONE_H
#define KEY_MATRIX_PHONE_H

#include "KeyMatrix.h"

class KeyMatrixPhone : public KeyMatrix {

public:
    static const char *_alphaMap[];

public:
    char *_buff;
    int _len;
    int _cur;
    char _mode;
    char _modeLock;
    char _lastKey;
    int _timer;
    int _charIndex;

public:
    KeyMatrixPhone(char *keymap, byte rows, byte cols, byte *rowPins, byte *colPins);
    bool pollEvent();
    void startTextMode(char *buffer, int length, char mode = KM_MODE_LOWER, bool modeLock = false);
    void stopTextMode();
    int cursor();
    char mode();

};

#endif
