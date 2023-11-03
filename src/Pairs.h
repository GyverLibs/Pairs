#pragma once

#include <Arduino.h>

#include "PairsExt.h"

class Pairs : public PairsExt {
   public:
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

    bool set(const PairsUtils::AnyText& key, const PairsUtils::AnyValue& value) {
        return PairsExt::set(key, value);
    }

    // установить по паре
    bool set(Pair_t pair, const PairsUtils::AnyValue& value) {
        if (!str || !pair.val) return 0;
        int16_t dif = value.len - pair.val_len;
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
    Pair_t add(const PairsUtils::AnyText& key, const PairsUtils::AnyValue& value) {
        if (!str) return Pair_t();
        uint16_t nlen = _len + !!_len + key.len + value.len + 3;
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