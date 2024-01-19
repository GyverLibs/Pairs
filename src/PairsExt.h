#pragma once

#include <Arduino.h>
#include <StringUtils.h>

#include "Pair_t.h"

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
    bool contains(sutil::AnyText key) {
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
    bool set(sutil::AnyText key, const sutil::AnyValue& value) {
        if (!str || !key.valid() || !value.valid()) return 0;
        Pair_t pair = get(key);
        if (pair.val) return set(pair, value);
        else return add(key, value).val;
    }

    // установить по паре
    virtual bool set(Pair_t pair, const sutil::AnyValue& value) {
        if (!str || !pair.val || !value.valid()) return 0;

        sutil::AnyText valuenc = value;
        int16_t dif = valuenc.length() - pair.val_len;
        if (dif > 0 && _len + dif >= size) return 0;

        if (valuenc.length() == pair.val_len && valuenc.compare(pair.val)) return 0;  // same
        uint16_t count = str + _len - (pair.val + pair.val_len) + 1;                  // +1 for \0
        memmove((void*)(pair.val + valuenc.length()), pair.val + pair.val_len, count);
        value.toStr((char*)pair.val, -1, false);
        _len += (valuenc.length() - pair.val_len);
        _changed = 1;
        return 1;
    }

    // установить по индексу
    bool set(int idx, const sutil::AnyValue& value) {
        if (str) {
            Pair_t pair = get(idx);
            if (pair.val) return set(pair, value);
        }
        return 0;
    }

    // добавить новую пару
    virtual Pair_t add(sutil::AnyText key, const sutil::AnyValue& value) {
        if (!str || !key.valid() || !value.valid()) return Pair_t();
        uint16_t nlen = _len + !!_len + key.length() + value.readLen() + 3;
        if (nlen >= size) return Pair_t();

        Pair_t pair;
        char* p = str + _len;
        if (_len) *(p++) = '\n';  // += \n
        *(p++) = '\"';
        pair.key = p;
        p += key.toStr(p);
        *(p++) = '\"';
        *(p++) = ':';
        pair.val = p;
        p += value.toStr(p);
        *p = '\0';
        pair.key_len = key.length();
        pair.val_len = value.readLen();
        _len = nlen;
        _changed = 1;
        _amount++;
        return pair;
    }

    // ======================= GET =======================

    // получить по ключу
    Pair_t get(sutil::AnyText key) {
        if (!str || !_len || !key.valid()) return Pair_t();
        const char* p = str;
        Pair_t pair;
        pair.key_len = key.length();

        while (1) {
            if (p[0] != '\"') return Pair_t();
            pair.key = p + 1;
            p = strchr(p + 1, '\"');
            if (!p) return Pair_t();
            if (((uint16_t)(p - pair.key) == key.length()) && p[1] == ':' && key.compare(pair.key)) {
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
    Pair_t get(int idx) {
        if (!str || idx >= (int)_amount) return Pair_t();
        const char* p = str + 1;  // skip 1st
        Pair_t pair;
        int i = 0;
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
        if (_pair.key) set(_pair, val);
        _pair = Pair_t();
    }

    // ======================== REMOVE ========================

    // удалить по ключу
    bool remove(sutil::AnyText key) {
        return remove(get(key));
    }

    // удалить по индексу
    bool remove(int idx) {
        return remove(get(idx));
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

    // deprecated
    bool removeN(int idx) {
        return remove(idx);
    }
    Pair_t getN(int idx) {
        return get(idx);
    }
    bool setN(int idx, const sutil::AnyValue& value) {
        return set(idx, value);
    }

   protected:
    uint16_t _len = 0;
    uint16_t _amount = 0;
    bool _changed = false;
    Pair_t _pair;

   private:
    PairsExt& _get(sutil::AnyText key) {
        _pair = get(key);
        if (!_pair.key) _pair = add(key, "");
        sutil::AnyText::_str = _pair.val;
        sutil::AnyText::_len = _pair.val_len;
        return *this;
    }
};