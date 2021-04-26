#include "KeyMatrixPhone.h"

static const char *KeyMatrixPhone::_alphaMap[] = {" !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

KeyMatrixPhone::KeyMatrixPhone(char *keymap, byte rows, byte cols, byte *rowPins, byte *colPins) :
KeyMatrix(keymap, rows, cols, rowPins, colPins),
_buff(NULL),
_len(0),
_cur(-1),
_mode(0),
_lastKey(0),
_timer(-1),
_charIndex(0) { }

boolean KeyMatrixPhone::pollEvent() {
    if (KeyMatrix::pollEvent()) {
        if (event.type == KM_KEYDOWN && _buff) {
            int t = millis();
            if (_timer != -1 && (t - _timer > 1000 || event.c != _lastKey)) {
                if (_cur + 2 < _len) {
                    _cur++;
                }
                _timer = -1;
            }
            _lastKey = event.c;
            if (event.c == '*') {
                // backspace
                if (_buff[_cur] == '\0' && _cur > 0) {
                    _cur--;
                }
                _buff[_cur] = '\0';
                _timer = -1;
                event.type = KM_TEXT;
                event.c = '\b';
            } else if (!_modeLock && event.c == '#') {
                // mode select
                _mode = (_mode + 1)%3;
                event.type = KM_MODE;
                event.c = _mode;
            } else if (_mode == KM_MODE_NUM) {
                if (event.c >= '0' && event.c <= '9') {
                    _buff[_cur] = event.c;
                    _buff[_cur + 1] = '\0';
                    if (_cur + 2 < _len) {
                        _cur++;
                    }
                    _timer = -1;
                    event.type = KM_TEXT;
                }
            } else if (event.c >= '1' && event.c <= '9') {
                int i = event.c - '1';
                if (_timer != -1) {
                    _charIndex++;
                    if (_alphaMap[i][_charIndex] == '\0') {
                        _charIndex = 0;
                    }
                } else {
                    _charIndex = 0;
                }
                char c = _alphaMap[i][_charIndex];
                if (_mode == KM_MODE_UPPER && c >= 'a' && c <= 'z') {
                    c += 'A' - 'a';
                }
                _buff[_cur] = c;
                _buff[_cur + 1] = '\0';
                _timer = t;
                event.type = KM_TEXT;
                event.c = _buff[_cur];
            }
        }
        return true;
    }
    return false;
}

void KeyMatrixPhone::startTextMode(char *buffer, int length, char mode, bool modeLock) {
    _buff = buffer;
    _len = length;
    _cur = 0;
    _buff[0] = '\0';
    _mode = mode;
    _modeLock = modeLock;
}

void KeyMatrixPhone::stopTextMode() {
    _buff = NULL;
    _len = 0;
    _cur = -1;
    _mode = KM_MODE_NONE;
}

int KeyMatrixPhone::cursor() {
    return _cur;
}

char KeyMatrixPhone::mode() {
    return _mode;
}
