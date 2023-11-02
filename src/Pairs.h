#pragma once

#include <Arduino.h>

#include "utils/PairsString.h"
#include "utils/utils.h"

class Pairs {
   private:
    bool _changed = false;
    Pair_t _pair;

   public:
    Pairs(uint16_t size = 0) {
        if (size) reserve(size);
    }

    Pairs(String& str, uint16_t size = 0) : str(str) {
        if (size) reserve(size);
    }

    String str;

    // ======================== SYSTEM ========================

    // зарезервировать строку
    bool reserve(uint16_t len) {
        return str.reserve(len);
    }

    // очистить строку
    void clear() {
        str = "";
    }

    // было изменение данных. Само сбросится в false
    bool changed() {
        return _changed ? (_changed = 0, true) : false;
    }

    // установить флаг на изменение
    void forceChange() {
        _changed = 1;
    }

    // ======================== SET ========================

    // установить по ключу
    bool set(const PairsUtils::AnyText& key, const PairsUtils::AnyValue& value, bool forcePGM = 0) {
        bool ch = PairsString::set_by_key(str, key.str, value.str, key.pgm || forcePGM);
        _changed |= ch;
        return ch;
    }

    // установить по индексу
    bool setN(uint16_t idx, const PairsUtils::AnyValue& value) {
        return PairsString::set_by_idx(str, idx, value.str);
    }

    // ======================== GET ========================

    // получить по ключу
    Pair_t get(const PairsUtils::AnyText& key, bool forcePGM = 0) {
        return PairsString::get_pair_by_key(str, key.str, key.pgm || forcePGM);
    }

    // получить по индексу
    Pair_t getN(uint16_t idx) {
        return PairsString::get_pair_by_idx(str, idx);
    }

    // ======================== [] ========================

    Pairs& operator[](uint16_t idx) {
        _pair = PairsString::get_pair_by_idx(str, idx);
        return *this;
    }
    Pairs& operator[](const PairsUtils::AnyText& key) {
        _pair = PairsString::get_pair_by_key(str, key.str, key.pgm);
        if (!_pair.key) _pair = PairsString::add_pair(str, key.str, "", key.pgm);
        return *this;
    }

    template <typename T>
    void operator=(T val) {
        if (_pair.key) {
            PairsUtils::AnyValue value(val);
            if (PairsString::set_by_pair(str, _pair, value.str)) _changed = true;
        }
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

    // ======================== MISC ========================

    // проверка на существование
    bool contains(const PairsUtils::AnyText& key, bool forcePGM = 0) {
        return PairsString::count_pairs(str, key.str, key.pgm || forcePGM);
    }

    // получить количество пар
    uint16_t length() {
        return PairsString::count_pairs(str);
    }

    // ======================== REMOVE ========================

    // удалить по ключу
    bool remove(const PairsUtils::AnyText& key, bool forcePGM = 0) {
        return PairsString::remove_by_key(str, key.str, key.pgm || forcePGM);
    }

    // удалить по индексу
    bool removeN(uint16_t idx) {
        return PairsString::remove_by_idx(str, idx);
    }
};

#pragma once