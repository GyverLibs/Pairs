#pragma once

#include <Arduino.h>
#include <StringUtils.h>

#include "Pair.h"
#include "PairUtils.h"

class PairsExt : public sutil::AnyText {
    friend class pairs::PairAccess;

   public:
    PairsExt() {}
    PairsExt(char* str, uint16_t size) : _size(size) {
        _str = str;
    }

    // ======================== SYSTEM ========================

    // получить размер буфера
    uint16_t getSize() {
        return _size;
    }

    // подключить буфер
    void setBuffer(char* str, uint16_t size) {
        _str = str;
        _size = _size;
    }

    // очистить строку
    void clear() {
        if (_str && _size) *((char*)_str) = '\0';
        _amount = _len = 0;
    }

    // было изменение данных. Само сбросится в false
    bool changed() {
        return _changed ? (_changed = 0, true) : false;
    }

    // проверка на существование пары
    bool contains(const sutil::AnyText& key) {
        if (!length() || !key.length()) return 0;
        char buf[key.length() + 3 + 1];  // "": == 3
        char* p = buf;
        *p++ = '\"';
        p += key.toStr(p);
        *p++ = '\"';
        *p++ = ':';
        *p++ = '\0';
        return strstr(_str, buf);
    }

    // количество пар
    uint16_t amount() {
        return _amount;
    }

    // пересчитать длину строки и количество пар (после ручных изменений в базе)
    void refresh() {
        if (!_str) return;
        calcLen();
        char* p = (char*)_str;
        uint16_t i = 1;
        while (1) {
            p = strchr(p + 1, '\"');  // skip 1st
            if (!p) break;
            if (p[-1] != '\\') i++;
        }
        _amount = i / 2;
    }

    // создать из текста
    bool fromText(const sutil::AnyText& text) {
        if (text.toStr((char*)_str, _size)) {
            refresh();
            _len = text.length();
            return 1;
        }
        return 0;
    }

    void operator=(const sutil::AnyText& text) {
        fromText(text);
    }

    // ======================== SET ========================

    // установить по ключу
    bool set(const sutil::AnyText& key, void* var, size_t size) {
        return set(key, pairs::Value(var, size));
    }
    // установить по ключу
    bool set(const sutil::AnyText& key, const pairs::Value& value) {
        if (!_str || !key.valid() || !value.valid()) return 0;

        Pair pair = get(key);
        if (pair.valid()) return set(pair, value);
        else return add(key, value).valid();
    }

    // установить по паре
    bool set(const Pair& pair, void* var, size_t size) {
        return set(pair, pairs::Value(var, size));
    }
    // установить по паре
    virtual bool set(const Pair& pair, const pairs::Value& value) {
        if (!_str || !pair.valid() || !pair.key.valid() || !value.valid()) return 0;

        int16_t dif = value.length() - pair.length();
        if (dif > 0 && length() + dif >= _size) return 0;
        if (value.compare(pair)) return 0;        // same
        uint16_t count = end() - pair.end() + 1;  // +1 for \0
        memmove((void*)(pair._str + value.length()), pair.end(), count);
        value.toStr((char*)pair._str, -1, false);
        _len += (value.length() - pair.length());
        _changed = 1;
        return 1;
    }

    // установить по индексу
    bool set(int idx, void* var, size_t size) {
        return set(idx, pairs::Value(var, size));
    }
    // установить по индексу
    bool set(int idx, const pairs::Value& value) {
        if (!_str || !value.valid()) return 0;
        Pair pair = get(idx);
        return pair.valid() ? set(pair, value) : 0;
    }

    // добавить новую пару
    Pair add(const sutil::AnyText& key, void* var, size_t size) {
        return add(key, pairs::Value(var, size));
    }
    // добавить новую пару
    virtual Pair add(const sutil::AnyText& key, const pairs::Value& value) {
        if (!_str || !key.valid() || !value.valid()) return Pair();

        uint16_t nlen = (length() ? (length() + 1) : 0) + key.length() + value.length() + 3;
        if (nlen >= _size) return Pair();

        Pair pair;
        char* p = (char*)end();
        if (length()) *(p++) = '\n';  // += \n
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
        if (!length() || !key.valid()) return Pair();

        const char* p = _str;
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
                        pair._len = end() - pair._str;
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
        if (!length() || idx < 0 || (uint16_t)idx >= _amount || _str[0] != '\"') return Pair();

        const char* p = _str + 1;  // skip 1st
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
                if (p && p[-1] == '\\') continue;

                if (i == idx) {
                    pair._len = p ? (p - pair._str - 1) : (end() - pair._str);
                    return pair;
                } else break;
            }
            p++;
            i++;
        }
        return Pair();
    }

    // ======================== [] ========================

    pairs::PairAccess operator[](int idx) {
        return pairs::PairAccess(this, _set, get(idx));
    }
    pairs::PairAccess operator[](const char* key) {
        return pairs::PairAccess(this, _set, _get_add(key));
    }
    pairs::PairAccess operator[](const __FlashStringHelper* key) {
        return pairs::PairAccess(this, _set, _get_add(key));
    }
    pairs::PairAccess operator[](const String& key) {
        return pairs::PairAccess(this, _set, _get_add(key));
    }
    pairs::PairAccess operator[](String& key) {
        return pairs::PairAccess(this, _set, _get_add(key));
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
        if (!length() || !pair.valid() || !pair.key.valid() || !_amount) return 0;

        if (!*pair.end()) {                     // last
            if ((pair.key._str - 1) == _str) {  // first
                *((char*)_str) = '\0';
                _len = 0;
            } else {
                *((char*)pair.key._str - 2) = '\0';
                _len = pair.key._str - 2 - _str;
            }
        } else {
            char* src = (char*)pair.end() + 1;
            char* dest = (char*)pair.key._str - 1;
            memmove((void*)dest, src, end() - src + 1);
            _len -= (src - dest);
        }
        _amount--;
        _changed = 1;
        return 1;
    }

    // совместимость
    bool tick() { return 0; }
    bool update() { return 1; }
    bool begin(uint16_t res = 0) { return 1; }
    void setTimeout(uint32_t tout = 10000) {}

    // deprecated
    bool removeN(int idx) {
        return remove(idx);
    }
    Pair getN(int idx) {
        return get(idx);
    }
    bool setN(int idx, const pairs::Value& value) {
        return set(idx, value);
    }

   protected:
    uint16_t _size = 0;
    uint16_t _amount = 0;
    bool _changed = false;

   private:
    static void _set(void* pairs, const Pair& pair, const pairs::Value& value) {
        ((PairsExt*)pairs)->set(pair, value);
    }
    Pair _get_add(const sutil::AnyText& key) {
        Pair pair = get(key);
        return pair.valid() ? pair : add(key, "");
    }
};