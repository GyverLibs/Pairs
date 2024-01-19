#pragma once
#include <Arduino.h>

struct Pair_t {
    const char* key = nullptr;
    uint16_t key_len = 0;
    const char* val = nullptr;
    uint16_t val_len = 0;
};