# KeyMatrix

KeyMatrix is poll event Arduino library for matrix keypads.

## Example Usage

    // initialize the keypad with the Arduino pins (AK-304 is a common phone keypad)
    AK_304(keypad, 4, 5, 6, 7, 8, 9, 10);

    // OR for other phone keypads
    KEY_MATRIX_PHONE(keypad, r0, r1, r2, r3, c0, c1, c2)

    // OR for generic keypads (up to 32 keys)
    char keymap[2][3] = {{'a', 'b', 'c'}, {'d', 'e', 'f'}};
    byte rowPins[2] = {r0, r1};
    byte colPins[3] = {c0, c1, c2};
    KeyMatrix keypad(keymap, 2, 3, rowPins, colPins);

    void loop() {
        // check for pending event
        if (keypad.pollEvent()) {
            // event data is stored on keypad.event
            if (keypad.event.type == KM_KEYDOWN && keypad.event.c == 'a') {
                // do something if key 'a' is pressed
            }
        }
    }

See [examples](examples) for more.
