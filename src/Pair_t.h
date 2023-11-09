#pragma once
#include <Arduino.h>

struct Pair_t {
    const char* key = nullptr;
    uint16_t key_len = 0;
    const char* val = nullptr;
    uint16_t val_len = 0;

    operator String() {
        return toString();
    }

    // вывести в int
    int32_t toInt() {
        return val ? atol(val) : 0;
    }

    // вывести в float
    float toFloat() {
        return val ? atof(val) : 0.0;
    }

    // вывести в String
    String toString(bool unescape = true) {
        if (!val) return String();
        String s;
        s.reserve(val_len);
        for (uint16_t i = 0; i < val_len; i++) {
            if (unescape && val[i] == '\\') continue;
            s += val[i];
        }
        return s;
    }

    // вывести в char массив
    bool toChar(char* buf, uint16_t len) {
        if (!val || val_len >= len) return 0;
        memcpy(buf, val, val_len);
        buf[val_len] = '\0';
        return 1;
    }
};