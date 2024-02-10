#pragma once

#include <Arduino.h>

#include "PairsExt.h"

class Pairs : public PairsExt {
   public:
    using PairsExt::set;

    Pairs(uint16_t size = 0) {
        reserve(size);  // create anyway
    }

    ~Pairs() {
        if (_str) free((void*)_str);
    }

    // ====================== SYSTEM  ======================

    // зарезервировать строку на количество символов
    bool reserve(uint16_t len) {
        len++;  // +/0
        if (_str && _size >= len) return 1;

        char* nstr = (char*)realloc((void*)_str, len);
        if (nstr) {
            _str = nstr;
            _size = len;
            if (!_len) *((char*)_str) = '\0';
            return 1;
        }
        return 0;
    }

    // создать из текста
    bool fromText(const sutil::AnyText& text) {
        if (!reserve(text.length())) return 0;
        return PairsExt::fromText(text);
    }

    void operator=(const sutil::AnyText& text) {
        fromText(text);
    }

    // ======================== SET ========================

    // установить по паре
    bool set(const Pair& pair, const pairs::Value& value) {
        if (!_str || !pair.valid() || !pair.key.valid() || !value.valid()) return 0;

        int16_t dif = value.length() - pair._len;
        if (dif > 0 && length() + dif >= _size) {
            Pair pairb = pair;
            char* strb = (char*)_str;
            if (!reserve(length() + dif)) return 0;
            pairb._str = _str + (pairb._str - strb);
            pairb.key._str = _str + (pairb.key._str - strb);
            return PairsExt::set(pairb, value);
        } else {
            return PairsExt::set(pair, value);
        }
    }

    // добавить новую пару
    Pair add(const sutil::AnyText& key, const pairs::Value& value) {
        if (!_str || !key.valid() || !value.valid()) return Pair();

        uint16_t nlen = (length() ? (length() + 1) : 0) + key.length() + value.length() + 3;
        if (nlen >= _size && !reserve(nlen)) return Pair();
        return PairsExt::add(key, value);
    }

   private:
    using PairsExt::setBuffer;

    bool _changeBuffer(uint16_t len) {
        char* nstr = (char*)realloc((void*)_str, len);
        if (nstr) {
            _str = nstr;
            _size = len;
            return 1;
        }
        return 0;
    }
};