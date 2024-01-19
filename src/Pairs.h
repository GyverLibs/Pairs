#pragma once

#include <Arduino.h>

#include "PairsExt.h"

class Pairs : public PairsExt {
   public:
    using PairsExt::set;

    Pairs(uint16_t size = 0) {
        reserve(size);
    }

    ~Pairs() {
        if (str) free(str);
    }

    // ====================== SYSTEM  ======================

    // зарезервировать строку
    bool reserve(uint16_t len) {
        if (str && size >= len + 1) return 1;
        if (_changeBuffer(len + 1)) {
            if (!_len) str[0] = '\0';
            return 1;
        }
        return 0;
    }

    // ======================== SET ========================

    // установить по паре
    bool set(Pair_t pair, const sutil::AnyValue& value) {
        if (!str || !pair.val || !value.valid()) return 0;
        int16_t dif = value.readLen() - pair.val_len;
        if (dif > 0 && _len + dif >= size) {
            Pair_t pairb = pair;
            char* strb = str;
            if (!reserve(_len + dif)) return 0;
            pair.val = str + (pairb.val - strb);
            pair.key = str + (pairb.key - strb);
        }
        return PairsExt::set(pair, value);
    }

    // добавить новую пару
    Pair_t add(sutil::AnyText key, const sutil::AnyValue& value) {
        if (!str || !key.valid() || !value.valid()) return Pair_t();
        uint16_t nlen = _len + !!_len + key.length() + value.readLen() + 3;
        if (nlen >= size && !reserve(nlen)) return Pair_t();
        return PairsExt::add(key, value);
    }

   private:
    using PairsExt::setBuffer;

    bool _changeBuffer(uint16_t len) {
        Pair_t pairb = _pair;
        char* nstr = (char*)realloc(str, len);
        if (nstr) {
            if (_pair.key && str != nstr) {
                _pair.val = nstr + (pairb.val - str);
                _pair.key = nstr + (pairb.key - str);
            }
            str = nstr;
            size = len;
            return 1;
        }
        return 0;
    }
};