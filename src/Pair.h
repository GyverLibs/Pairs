#pragma once
#include <Arduino.h>
#include <StringUtils.h>

class Pair : public sutil::AnyText {
   public:
    sutil::AnyText key;
};