#pragma once

#include <Arduino.h>

namespace PairsUtils {

class AnyText {
   public:
    AnyText() {}
    AnyText(const char* s, bool pgm) : str(s), pgm(pgm), len(pgm ? strlen_P(s) : strlen(s)) {}
    AnyText(const String& s) : str(s.c_str()), len(s.length()) {}
    AnyText(const char* s) : str(s), len(strlen(s)) {}
    AnyText(const __FlashStringHelper* s) : str((const char*)s), pgm(1), len(strlen_P((PGM_P)s)) {}

    bool cmp(const char* s) const {
        return pgm ? !strncmp_P(s, str, len) : !strncmp(s, str, len);
    }

    const char* str = nullptr;
    bool pgm = 0;
    uint16_t len = 0;
};

class AnyValue : public AnyText {
   public:
    AnyValue(const char* str) : AnyText(str) {}
    AnyValue(const String& str) : AnyText(str) {}
    AnyValue(const __FlashStringHelper* str) : AnyText(str) {}
    AnyValue(const char* s, bool pgm) : AnyText(s, pgm) {}

    AnyValue(int8_t value) {
        itoa(value, buf, DEC);
        len = strlen(buf);
        str = buf;
    }
    AnyValue(uint8_t value) {
        utoa(value, buf, DEC);
        len = strlen(buf);
        str = buf;
    }

    AnyValue(int16_t value) {
        itoa(value, buf, DEC);
        len = strlen(buf);
        str = buf;
    }
    AnyValue(uint16_t value) {
        utoa(value, buf, DEC);
        len = strlen(buf);
        str = buf;
    }

    AnyValue(int32_t value) {
        ltoa(value, buf, DEC);
        len = strlen(buf);
        str = buf;
    }
    AnyValue(uint32_t value) {
        ultoa(value, buf, DEC);
        len = strlen(buf);
        str = buf;
    }

    AnyValue(double value, uint8_t dec = 2) {
        dtostrf(value, (dec + 2), dec, buf);
        len = strlen(buf);
        str = buf;
    }

   private:
    char buf[20];
};

}  // namespace PairsUtils