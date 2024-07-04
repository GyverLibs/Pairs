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
        reset();
    }

    // ====================== SYSTEM  ======================

    // зарезервировать строку на количество символов
    bool reserve(uint16_t size) {
        size++;  // +/0
        if (_str && _size >= size) return 1;

        char* nstr = (char*)realloc((void*)_str, size);
        if (nstr) {
            _str = nstr;
            _size = size;
            if (!_len) *((char*)_str) = '\0';  // 1st
            return 1;
        }
        return 0;
    }

    // создать из текста
    bool fromText(const Text& text) {
        if (!reserve(text.length())) return 0;
        return PairsExt::fromText(text);
    }

    void operator=(const Text& text) {
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
    Pair add(const Text& key, const pairs::Value& value) {
        if (!_str || !key.valid() || !value.valid()) return Pair();

        uint16_t nlen = (length() ? (length() + 1) : 0) + key.length() + value.length() + 3;
        if (nlen >= _size && !reserve(nlen)) return Pair();
        return PairsExt::add(key, value);
    }

    Pairs(const Pairs& val) {
        copy(val);
    }
    Pairs& operator=(const Pairs& val) {
        copy(val);
        return *this;
    }

#if __cplusplus >= 201103L
    Pairs(PairsExt&&) = delete;
    Pairs& operator=(PairsExt&&) = delete;

    Pairs(Pairs&& rval) noexcept {
        move(rval);
    }
    Pairs& operator=(Pairs&& rval) noexcept {
        move(rval);
        return *this;
    }
#endif

    // копировать из другого экземпляра
    bool copy(const Pairs& val) {
        if (this == &val) return 1;
        if (!val._str || !val._len || !reserve(val._len)) {
            reset();
            reserve(0);  // init
            return 0;
        }
        memcpy((void*)_str, (void*)val._str, val._len + 1);
        // left: _str, _size
        _len = val._len;
        _amount = val._amount;
        _changed = val._changed;
        return 1;
    }

    // переместить из другого экземпляра
    void move(Pairs& rval) noexcept {
        if (this == &rval) return;
        if (_str) {
            if (rval._str && _size >= rval._len + 1) {
                memcpy((void*)_str, (void*)rval._str, rval._len + 1);
                // left: _str, _size
                _len = rval._len;
                _amount = rval._amount;
                _changed = rval._changed;
                return;
            } else {
                free((void*)_str);
            }
        }
        _str = rval._str;
        rval._str = nullptr;
        _size = rval._size;
        _len = rval._len;
        _amount = rval._amount;
        _changed = rval._changed;
    }

    // освободить память
    void reset() {
        free((void*)_str);
        _str = nullptr;
        _size = 0;
        _len = 0;
        _amount = 0;
        _changed = false;
    }

   protected:
    using PairsExt::_amount;
    using PairsExt::_changed;
    using PairsExt::_len;
    using PairsExt::_size;
    using PairsExt::_str;

   private:
    using PairsExt::setBuffer;
};