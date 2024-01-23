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
    bool set(Pair pair, const sutil::AnyValue& value) {
        if (!str || !pair.valid() || !pair.key.valid() || !value.valid()) return 0;
        int16_t dif = value.length() - pair._len;
        if (dif > 0 && _len + dif >= size) {
            Pair pairb = pair;
            char* strb = str;
            if (!reserve(_len + dif)) return 0;
            pair._str = str + (pairb._str - strb);
            pair.key._str = str + (pairb.key._str - strb);
        }
        return PairsExt::set(pair, value);
    }

    // добавить новую пару
    Pair add(const sutil::AnyText& key, const sutil::AnyValue& value) {
        if (!str || !key.valid() || !value.valid()) return Pair();
        uint16_t nlen = _len + !!_len + key.length() + value.length() + 3;
        if (nlen >= size && !reserve(nlen)) return Pair();
        return PairsExt::add(key, value);
    }

   private:
    using PairsExt::setBuffer;

    bool _changeBuffer(uint16_t len) {
        Pair pairb = _pair;
        char* nstr = (char*)realloc(str, len);
        if (nstr) {
            if (_pair.key.valid() && str != nstr) {
                _pair._str = nstr + (pairb._str - str);
                _pair.key._str = nstr + (pairb.key._str - str);
            }
            str = nstr;
            size = len;
            return 1;
        }
        return 0;
    }
};