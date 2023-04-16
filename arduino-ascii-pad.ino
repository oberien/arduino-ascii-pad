#include <Keyboard.h>
/*#include <Adafruit_NeoPixel.h>*/

#define ENTERL D7
#define ENTERR D10
#define ASCII1 D9
#define ASCII2 D5
#define ASCII4 D6
#define ASCII8 D4
#define ASCII16 D8
#define ASCII32 D3
#define ASCII64 D2

void setup() {
    Serial.begin(9600);
    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    pinMode(ENTERL, INPUT_PULLUP);
    pinMode(ENTERR, INPUT_PULLUP);
    pinMode(ASCII1, INPUT_PULLUP);
    pinMode(ASCII2, INPUT_PULLUP);
    pinMode(ASCII4, INPUT_PULLUP);
    pinMode(ASCII8, INPUT_PULLUP);
    pinMode(ASCII16, INPUT_PULLUP);
    pinMode(ASCII32, INPUT_PULLUP);
    pinMode(ASCII64, INPUT_PULLUP);
    Keyboard.begin();
}

bool ascii1 = false;
bool ascii2 = false;
bool ascii4 = false;
bool ascii8 = false;
bool ascii16 = false;

bool last1 = false;
bool last2 = false;
bool last4 = false;
bool last8 = false;
bool last16 = false;
bool last_enterl = false;
bool last_enterr = false;

bool handled;

unsigned long lastChangeTime = 0;
#define DEBOUNCE_DELAY 50

void loop() {
    bool current1 = !digitalRead(ASCII1);
    bool current2 = !digitalRead(ASCII2);
    bool current4 = !digitalRead(ASCII4);
    bool current8 = !digitalRead(ASCII8);
    bool current16 = !digitalRead(ASCII16);
    bool current32 = !digitalRead(ASCII32);
    bool current64 = !digitalRead(ASCII64);
    bool enterl = !digitalRead(ENTERL);
    bool enterr = !digitalRead(ENTERR);

    if (
        current1 != last1 || current2 != last2 || current4 != last4
        || current8 != last8 || current16 != last16
        || enterl != last_enterl || enterr != last_enterr
    ) {
        handled = false;
        lastChangeTime = millis();

        last1 = current1;
        last2 = current2;
        last4 = current4;
        last8 = current8;
        last16 = current16;
        last_enterl = enterl;
        last_enterr = enterr;
        return;
    }

    if (handled) {
      return;
    }

    if (millis() - lastChangeTime < DEBOUNCE_DELAY) {
      // wait for state to settle / wait for debounce
      return;
    }
    handled = true;

    ascii1 |= current1;
    ascii2 |= current2;
    ascii4 |= current4;
    ascii8 |= current8;
    ascii16 |= current16;

    if (
        (current1 == 1 || current2 == 1 || current4 == 1 || current8 == 1 || current16 == 1)
        && enterl == 0 && enterr == 0
    ) {
        return;
    }

    char ascii = (ascii1 << 0) | (ascii2 << 1) | (ascii4 << 2) | (ascii8 << 3)
        | (ascii16 << 4) | (current32 << 5) | (current64 << 6);

    if (ascii == 0) {
        return;
    }

    char s[30] = {0};
    sprintf(s, "%d%d%d%d%d%d%d    0x%02x    '%c'", current64, current32, ascii16, ascii8, ascii4, ascii2, ascii1, ascii, ascii);
    Serial.println(s);
    Keyboard.write(ascii);

    ascii1 = false;
    ascii2 = false;
    ascii4 = false;
    ascii8 = false;
    ascii16 = false;

    /*digitalWrite(LED_BUILTIN, HIGH);*/
    /*delay(500);*/
    /*digitalWrite(LED_BUILTIN, LOW);*/
    /*delay(500);*/

    /*morseLoop(ms);*/
    /*rainbowLoop(ms);*/
}

