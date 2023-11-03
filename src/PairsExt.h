#pragma once

#include <Arduino.h>

#include "Pair_t.h"
#include "utils/utils.h"

class PairsExt {
   public:
    PairsExt() {}
    PairsExt(char* str, uint16_t size) : str(str), size(size) {}

    char* str = nullptr;
    uint16_t size = 0;

    // ======================== SYSTEM ========================

    // подключить буфер
    void setBuffer(char* str, uint16_t size) {
        this->str = str;
        this->size = size;
    }

    // очистить строку
    void clear() {
        if (str) str[0] = '\0';
        _amount = _len = 0;
    }

    // было изменение данных. Само сбросится в false
    bool changed() {
        return _changed ? (_changed = 0, true) : false;
    }

    // проверка на существование пары
    bool contains(const PairsUtils::AnyText& key) {
        if (!str || !_len) return 0;
        char buf[key.len + 3 + 1];  // "": == 3
        key.pgm ? memcpy_P(buf + 1, key.str, key.len) : memcpy(buf + 1, key.str, key.len);
        buf[key.len + 1] = buf[0] = '\"';
        buf[key.len + 2] = ':';
        buf[key.len + 3] = '\0';
        return strstr(str, buf);
    }

    // фактическая длина строки
    uint16_t length() {
        return _len;
    }

    // количество пар
    uint16_t amount() {
        return _amount;
    }

    // пересчитать длину строки и количество пар (после ручных изменений в базе)
    void refresh() {
        if (!str) return;
        _len = strlen(str);
        char* p = str;
        uint16_t i = 1;
        while (1) {
            p = strchr(p + 1, '\"');  // skip 1st
            if (!p) break;
            if (p[-1] != '\\') i++;
        }
        _amount = (i >> 1);  // /2
    }

    // ======================== SET ========================

    // установить по ключу
    bool set(const PairsUtils::AnyText& key, const PairsUtils::AnyValue& value) {
        if (!str) return 0;
        Pair_t pair = get(key);
        if (pair.val) return set(pair, value);
        else return add(key, value).val;
    }

    // установить по паре
    virtual bool set(Pair_t pair, const PairsUtils::AnyValue& value) {
        if (!str || !pair.val) return 0;
        int16_t dif = value.len - pair.val_len;
        if (dif > 0 && _len + dif >= size) return 0;

        if (value.len == pair.val_len && value.cmp(pair.val)) return 0;  // same
        uint16_t count = str + _len - (pair.val + pair.val_len) + 1;     // +1 for \0
        memmove((void*)(pair.val + value.len), pair.val + pair.val_len, count);
        if (value.pgm) memcpy_P((void*)pair.val, value.str, value.len);
        else memcpy((void*)pair.val, value.str, value.len);
        _len += (value.len - pair.val_len);
        _changed = 1;
        return 1;
    }

    // установить по индексу
    bool setN(uint16_t idx, const PairsUtils::AnyValue& value) {
        if (str) {
            Pair_t pair = getN(idx);
            if (pair.val) return set(pair, value);
        }
        return 0;
    }

    // добавить новую пару
    virtual Pair_t add(const PairsUtils::AnyText& key, const PairsUtils::AnyValue& value) {
        if (!str) return Pair_t();
        uint16_t nlen = _len + !!_len + key.len + value.len + 3;
        if (nlen >= size) return Pair_t();

        Pair_t pair;
        char* p = str + _len;
        if (_len) *(p++) = '\n';  // += \n
        *(p++) = '\"';
        pair.key = p;
        if (key.pgm) memcpy_P(p, key.str, key.len);
        else memcpy(p, key.str, key.len);
        p += key.len;
        *(p++) = '\"';
        *(p++) = ':';
        pair.val = p;
        if (value.pgm) memcpy_P(p, value.str, value.len);
        else memcpy(p, value.str, value.len);
        p += value.len;
        *p = '\0';

        pair.key_len = key.len;
        pair.val_len = value.len;
        _len = nlen;
        _changed = 1;
        _amount++;
        return pair;
    }

    // ======================= GET =======================

    // получить по ключу
    Pair_t get(const PairsUtils::AnyText& key) {
        if (!str || !_len) return Pair_t();
        const char* p = str;
        Pair_t pair;
        pair.key_len = key.len;

        while (1) {
            if (p[0] != '\"') return Pair_t();
            pair.key = p + 1;
            p = strchr(p + 1, '\"');
            if (!p) return Pair_t();
            if ((p - pair.key == key.len) && p[1] == ':' && key.cmp(pair.key)) {
                p++;
                pair.val = p + 1;
                while (1) {
                    p = strchr(p + 1, '\"');
                    if (!p) {
                        pair.val_len = (str + _len) - pair.val;
                        return pair;
                    } else if (p[-1] != '\\') {
                        pair.val_len = p - pair.val - 1;  // -1 for div
                        return pair;
                    }
                }
            }
            while (1) {
                p = strchr(p + 1, '\"');
                if (!p) return Pair_t();
                if (p[-1] != '\\') break;
            }
        }
        return Pair_t();
    }

    // получить по индексу
    Pair_t getN(uint16_t idx) {
        if (!str || idx >= _amount) return Pair_t();
        const char* p = str + 1;  // skip 1st
        Pair_t pair;
        uint16_t i = 0;
        while (1) {
            pair.key = p;
            p = strchr(p + 1, '\"');
            if (!p) break;
            pair.key_len = p - pair.key;
            if (*(++p) != ':') break;

            pair.val = ++p;
            while (1) {
                p = strchr(p + 1, '\"');
                if (p[-1] == '\\') continue;

                if (i == idx) {
                    if (!p) pair.val_len = (str + _len) - pair.val;
                    else pair.val_len = p - pair.val - 1;  // -1 for div
                    return pair;
                } else break;
            }
            p++;
            i++;
        }
        return Pair_t();
    }

    // ======================== [] ========================

    PairsExt& operator[](uint16_t idx) {
        _pair = getN(idx);
        return *this;
    }
    PairsExt& operator[](const PairsUtils::AnyText& key) {
        _pair = get(key);
        if (!_pair.key) _pair = add(key, "");
        return *this;
    }

    template <typename T>  // TODO
    void operator=(T val) {
        if (_pair.key) set(_pair, PairsUtils::AnyValue(val));
        _pair = Pair_t();
    }

    // ======================== EXPORT ========================

    operator String() {
        return _pair.toString();
    }

    // вывести в int
    int32_t toInt() {
        return _pair.toInt();
    }

    // вывести в float
    float toFloat() {
        return _pair.toFloat();
    }

    // вывести в String
    String toString() {
        return _pair.toString();
    }

    // вывести в char массив
    bool toChar(char* buf, uint16_t len) {
        return _pair.toChar(buf, len);
    }

    // ======================== REMOVE ========================

    // удалить по ключу
    bool remove(const PairsUtils::AnyText& key) {
        return remove(get(key));
    }

    // удалить по индексу
    bool removeN(uint16_t idx) {
        return remove(getN(idx));
    }

    // удалить по паре
    bool remove(Pair_t pair) {
        if (!str || !pair.val || !_len) return 0;
        bool first = (pair.key - 1) == str;
        bool last = *(pair.val + pair.val_len) == '\0';
        if (last) {
            if (first) {
                *str = '\0';
                _len = 0;
            } else {
                *((char*)pair.key - 2) = '\0';
                _len = pair.key - 2 - str;
            }
        } else {
            char* end = str + _len;
            char* src = (char*)pair.val + pair.val_len + 1;
            char* dest = (char*)pair.key - 1;
            memmove((void*)dest, src, end - src + 1);
            _len -= (src - dest);
        }
        _amount--;
        _changed = 1;
        return 1;
    }

   protected:
    uint16_t _len = 0;
    uint16_t _amount = 0;
    bool _changed = false;
    Pair_t _pair;
};