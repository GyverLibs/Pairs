#pragma once

#include <Arduino.h>

#include "utils/PairsChar.h"
#include "utils/utils.h"

class PairsExt {
   private:
    bool _changed = false;
    Pair_t _pair;

   public:
    PairsExt() {}
    PairsExt(char* str, uint16_t size) : str(str), size(size) {}

    char* str;
    uint16_t size;

    // ======================== SYSTEM ========================

    // подключить буфер
    void setBuffer(char* str, uint16_t size) {
        this->str = str;
        this->size = size;
    }

    // очистить строку
    void clear() {
        str[0] = '\0';
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
        bool ch = PairsChar::set_by_key(str, size, key.str, value.str, key.pgm || forcePGM);
        _changed |= ch;
        return ch;
    }

    // установить по индексу
    bool setN(uint16_t idx, const PairsUtils::AnyValue& value) {
        return PairsChar::set_by_idx(str, size, idx, value.str);
    }

    // ======================= GET =======================

    // получить по ключу
    Pair_t get(const PairsUtils::AnyText& key, bool forcePGM = 0) {
        return PairsChar::get_pair_by_key(str, key.str, key.pgm || forcePGM);
    }

    // получить по индексу
    Pair_t getN(uint16_t idx) {
        return PairsChar::get_pair_by_idx(str, idx);
    }

    // ======================== [] ========================

    PairsExt& operator[](uint16_t idx) {
        _pair = PairsChar::get_pair_by_idx(str, idx);
        return *this;
    }
    PairsExt& operator[](const PairsUtils::AnyText& key) {
        _pair = PairsChar::get_pair_by_key(str, key.str, key.pgm);
        if (!_pair.key) _pair = PairsChar::add_pair(str, size, key.str, "", key.pgm);
        return *this;
    }

    template <typename T>
    void operator=(T val) {
        if (_pair.key) {
            PairsUtils::AnyValue value(val);
            if (PairsChar::set_by_pair(str, size, _pair, value.str)) _changed = 1;
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

    // ======================== MICS ========================

    // проверка на существование
    bool contains(const PairsUtils::AnyText& key, bool forcePGM = 0) {
        return PairsChar::count_pairs(str, key.str, key.pgm || forcePGM);
    }

    // получить количество пар
    uint16_t length() {
        return PairsChar::count_pairs(str);
    }

    // ======================== REMOVE ========================

    // удалить по ключу
    bool remove(const PairsUtils::AnyText& key, bool forcePGM = 0) {
        return PairsChar::remove_by_key(str, key.str, key.pgm || forcePGM);
    }

    // удалить по индексу
    bool removeN(uint16_t idx) {
        return PairsChar::remove_by_idx(str, idx);
    }
};