#pragma once

#include <Arduino.h>

#include "PairsExt.h"

template <uint16_t SIZE>
class PairsStatic : public PairsExt {
   private:
    char buf[SIZE + 1] = {0};

   public:
    PairsStatic() : PairsExt(buf, SIZE + 1) {}
};