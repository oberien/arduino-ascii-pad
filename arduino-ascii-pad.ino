#include <Keyboard.h>
/*#include <Adafruit_NeoPixel.h>*/

class Switch {
    private:
    unsigned long lastChangeMillis = 0;
    int pin = -1;
    bool lastBouncingValue = false;

    public:
    bool justPressed = false;
    bool justReleased = false;
    bool pressed = false;
    bool released = true;
    bool value = false;

    Switch(int pin, int inputMode);

    void update();
};

Switch::Switch(int pin, int inputMode) {
    this->pin = pin;
    pinMode(pin, inputMode);
}

void Switch::update() {
    justPressed = false;
    justReleased = false;

    bool bouncingValue = !digitalRead(pin);
    if (bouncingValue != lastBouncingValue) {
        lastChangeMillis = millis();
        lastBouncingValue = bouncingValue;
    }
    if (millis() - lastChangeMillis < 20) {
        return;
    }

    if (value != bouncingValue) {
        if (value) {
            justReleased = true;
        } else {
            justPressed = true;
        }
        value = bouncingValue;
        pressed = value;
        released = !value;
    }
}

#define ENTERL D7
#define ENTERR D10
#define ASCII1 D9
#define ASCII2 D5
#define ASCII4 D6
#define ASCII8 D4
#define ASCII16 D8
#define ASCII32 D3
#define ASCII64 D2

Switch switchEnterL(ENTERL, INPUT_PULLUP);
Switch switchEnterR(ENTERR, INPUT_PULLUP);
Switch switch1(ASCII1, INPUT_PULLUP);
Switch switch2(ASCII2, INPUT_PULLUP);
Switch switch4(ASCII4, INPUT_PULLUP);
Switch switch8(ASCII8, INPUT_PULLUP);
Switch switch16(ASCII16, INPUT_PULLUP);
Switch switch32(ASCII32, INPUT_PULLUP);
Switch switch64(ASCII64, INPUT_PULLUP);

void setup() {
    Serial.begin(9600);
    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    Keyboard.begin();
}

bool ascii1 = false;
bool ascii2 = false;
bool ascii4 = false;
bool ascii8 = false;
bool ascii16 = false;

void loop() {
    switch1.update();
    switch2.update();
    switch4.update();
    switch8.update();
    switch16.update();
    switch32.update();
    switch64.update();
    switchEnterL.update();
    switchEnterR.update();

    ascii1 |= switch1.value;
    ascii2 |= switch2.value;
    ascii4 |= switch4.value;
    ascii8 |= switch8.value;
    ascii16 |= switch16.value;

    bool anyJustReleased = switch1.justReleased || switch2.justReleased || switch4.justReleased || switch8.justReleased || switch16.justReleased;
    bool allReleased = switch1.released && switch2.released && switch4.released && switch8.released && switch16.released;
    bool enterJustPressed = switchEnterL.justPressed || switchEnterR.justPressed;

    if (anyJustReleased && allReleased || enterJustPressed) {
        char ascii = (ascii1 << 0) | (ascii2 << 1) | (ascii4 << 2) | (ascii8 << 3)
            | (ascii16 << 4) | (switch32.value << 5) | (switch64.value << 6);

        if (ascii == 0) {
            return;
        }

        char s[30] = {0};
        sprintf(s, "%d%d%d%d%d%d%d    0x%02x    '%c'", switch64.value, switch32.value, ascii16, ascii8, ascii4, ascii2, ascii1, ascii, ascii);
        Serial.println(s);
        Keyboard.write(ascii);

        ascii1 = false;
        ascii2 = false;
        ascii4 = false;
        ascii8 = false;
        ascii16 = false;
    }
}


