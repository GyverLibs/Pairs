#pragma once

#include <Arduino.h>

#include "PairsExt.h"

// PAIRS_NO_FILE

template <uint16_t SIZE>
class PairsStatic : public PairsExt {
   private:
    bool _changed = false;
    Pair_t _pair;

   public:
    PairsStatic() : PairsExt(str, SIZE + 1) {}

    // #if (defined(ESP8266) || defined(ESP32)) && defined(FS_H) && !defined(PAIRS_NO_FILE)
    //     PairsStatic(File f) : PairsExt(str, SIZE + 1) {
    //       if (f.size() > SIZE);
    //     }
    // #endif

    char str[SIZE + 1] = {0};
};