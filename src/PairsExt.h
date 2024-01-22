#pragma once

#include <Arduino.h>
#include <StringUtils.h>

#include "Pair.h"

class PairsExt : public sutil::AnyText {
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
    bool contains(const sutil::AnyText& key) {
        if (!str || !_len || !key.valid()) return 0;
        char buf[key.length() + 3 + 1];  // "": == 3
        char* p = buf;
        *p++ = '\"';
        p += key.toStr(p);
        *p++ = '\"';
        *p++ = ':';
        *p++ = '\0';
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
    bool set(const sutil::AnyText& key, const sutil::AnyValue& value) {
        if (!str || !key.valid() || !value.valid()) return 0;
        Pair pair = get(key);
        if (pair._str) return set(pair, value);
        else return add(key, value)._str;
    }

    // установить по паре
    virtual bool set(Pair pair, const sutil::AnyValue& value) {
        if (!str || !pair._str || !value.valid()) return 0;

        int16_t dif = value.length() - pair._len;
        if (dif > 0 && _len + dif >= size) return 0;

        if (value.compare(pair)) return 0;                          // same
        uint16_t count = str + _len - (pair._str + pair._len) + 1;  // +1 for \0
        memmove((void*)(pair._str + value.length()), pair._str + pair._len, count);
        value.toStr((char*)pair._str, -1, false);
        _len += (value.length() - pair._len);
        _changed = 1;
        return 1;
    }

    // установить по индексу
    bool set(int idx, const sutil::AnyValue& value) {
        if (str) {
            Pair pair = get(idx);
            if (pair._str) return set(pair, value);
        }
        return 0;
    }

    // добавить новую пару
    virtual Pair add(const sutil::AnyText& key, const sutil::AnyValue& value) {
        if (!str || !key.valid() || !value.valid()) return Pair();
        uint16_t nlen = _len + !!_len + key.length() + value.length() + 3;
        if (nlen >= size) return Pair();

        Pair pair;
        char* p = str + _len;
        if (_len) *(p++) = '\n';  // += \n
        *(p++) = '\"';
        pair.key._str = p;
        p += key.toStr(p);
        *(p++) = '\"';
        *(p++) = ':';
        pair._str = p;
        p += value.toStr(p);
        *p = '\0';
        pair.key._len = key.length();
        pair._len = value.length();
        _len = nlen;
        _changed = 1;
        _amount++;
        return pair;
    }

    // ======================= GET =======================

    // получить по ключу
    Pair get(const sutil::AnyText& key) {
        if (!str || !_len || !key.valid()) return Pair();
        const char* p = str;
        Pair pair;
        pair.key._len = key.length();

        while (1) {
            if (p[0] != '\"') return Pair();
            pair.key._str = p + 1;
            p = strchr(p + 1, '\"');
            if (!p) return Pair();
            if (((uint16_t)(p - pair.key._str) == key.length()) && p[1] == ':' && key.compare(pair.key)) {
                p++;
                pair._str = p + 1;
                while (1) {
                    p = strchr(p + 1, '\"');
                    if (!p) {
                        pair._len = (str + _len) - pair._str;
                        return pair;
                    } else if (p[-1] != '\\') {
                        pair._len = p - pair._str - 1;  // -1 for div
                        return pair;
                    }
                }
            }
            while (1) {
                p = strchr(p + 1, '\"');
                if (!p) return Pair();
                if (p[-1] != '\\') break;
            }
        }
        return Pair();
    }

    // получить по индексу
    Pair get(int idx) {
        if (!str || idx >= (int)_amount) return Pair();
        const char* p = str + 1;  // skip 1st
        Pair pair;
        int i = 0;
        while (1) {
            pair.key._str = p;
            p = strchr(p + 1, '\"');
            if (!p) break;
            pair.key._len = p - pair.key._str;
            if (*(++p) != ':') break;

            pair._str = ++p;
            while (1) {
                p = strchr(p + 1, '\"');
                if (p[-1] == '\\') continue;

                if (i == idx) {
                    if (!p) pair._len = (str + _len) - pair._str;
                    else pair._len = p - pair._str - 1;  // -1 for div
                    return pair;
                } else break;
            }
            p++;
            i++;
        }
        return Pair();
    }

    // ======================== [] ========================

    PairsExt& operator[](int idx) {
        _pair = get(idx);
        return *this;
    }
    PairsExt& operator[](const char* key) {
        return _get(key);
    }
    PairsExt& operator[](const __FlashStringHelper* key) {
        return _get(key);
    }
    PairsExt& operator[](const String& key) {
        return _get(key);
    }
    PairsExt& operator[](String& key) {
        return _get(key);
    }

    void operator=(const sutil::AnyValue& val) {
        if (_pair.key._str) set(_pair, val);
        _pair = Pair();
    }

    // ======================== REMOVE ========================

    // удалить по ключу
    bool remove(const sutil::AnyText& key) {
        return remove(get(key));
    }

    // удалить по индексу
    bool remove(int idx) {
        return remove(get(idx));
    }

    // удалить по паре
    bool remove(Pair pair) {
        if (!str || !pair._str || !_len) return 0;
        bool first = (pair.key._str - 1) == str;
        bool last = *(pair._str + pair._len) == '\0';
        if (last) {
            if (first) {
                *str = '\0';
                _len = 0;
            } else {
                *((char*)pair.key._str - 2) = '\0';
                _len = pair.key._str - 2 - str;
            }
        } else {
            char* end = str + _len;
            char* src = (char*)pair._str + pair._len + 1;
            char* dest = (char*)pair.key._str - 1;
            memmove((void*)dest, src, end - src + 1);
            _len -= (src - dest);
        }
        _amount--;
        _changed = 1;
        return 1;
    }

    // deprecated
    bool removeN(int idx) {
        return remove(idx);
    }
    Pair getN(int idx) {
        return get(idx);
    }
    bool setN(int idx, const sutil::AnyValue& value) {
        return set(idx, value);
    }

   protected:
    uint16_t _len = 0;
    uint16_t _amount = 0;
    bool _changed = false;
    Pair _pair;

    PairsExt& _get(const sutil::AnyText& key) {
        _pair = get(key);
        if (!_pair.key._str) _pair = add(key, "");
        sutil::AnyText::_str = _pair._str;
        sutil::AnyText::_len = _pair._len;
        return *this;
    }
};