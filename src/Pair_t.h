#pragma once
#include <Arduino.h>

struct Pair_t {
    const char* key = nullptr;
    uint16_t key_len = 0;
    const char* val = nullptr;
    uint16_t val_len = 0;

    // вывести в int
    int32_t toInt() {
        return atol(val);
    }

    // вывести в float
    float toFloat() {
        return atof(val);
    }

    // вывести в String
    String toString() {
        String s;
        s.reserve(val_len);
        for (uint16_t i = 0; i < val_len; i++) s += val[i];
        return s;
    }

    // вывести в char массив
    bool toChar(char* buf, uint16_t len) {
        if (val_len >= len) return 0;
        memcpy(buf, val, val_len);
        buf[val_len] = '\0';
        return 1;
    }
};