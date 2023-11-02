#pragma once

#include <Arduino.h>

namespace PairsUtils {

class AnyValue {
   public:
    AnyValue(const char* str) : str(str) {}
    // AnyValue(const __FlashStringHelper* str) : str((const char*)str), pgm(1) {}
    AnyValue(const String& str) : str(str.c_str()) {}

    AnyValue(int8_t value) {
        itoa(value, buf, DEC);
    }
    AnyValue(uint8_t value) {
        utoa(value, buf, DEC);
    }

    AnyValue(int16_t value) {
        itoa(value, buf, DEC);
    }
    AnyValue(uint16_t value) {
        utoa(value, buf, DEC);
    }

    AnyValue(int32_t value) {
        ltoa(value, buf, DEC);
    }
    AnyValue(uint32_t value) {
        ultoa(value, buf, DEC);
    }

    AnyValue(double value, uint8_t dec = 2) {
        dtostrf(value, (dec + 2), dec, buf);
    }

    char buf[30];
    const char* str = buf;
    // bool pgm = false;
};

class AnyText {
   public:
    AnyText() {}
    AnyText(const String& s) : str(s.c_str()) {}
    AnyText(const char* s) : str(s) {}
    AnyText(const __FlashStringHelper* s) : str((const char*)s), pgm(1) {}

    const char* str = nullptr;
    bool pgm = 0;
};

}