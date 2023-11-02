#pragma once
#include <Arduino.h>

#include "Pair_t.h"

namespace PairsChar {
// ======================== GET ========================

// получить пару по ключу
Pair_t get_pair_by_key(const char* str, const char* key, bool pgm = false);

// получить пару по индексу
Pair_t get_pair_by_idx(const char* str, uint16_t idx);

// ======================== SET ========================

// установить значение по ключу(true - добавлено или изменено)
bool set_by_key(char* str, uint16_t size, const char* key, const char* value, bool pgm = false);

// установить значение по паре (true - добавлено или изменено)
bool set_by_pair(char* str, uint16_t size, Pair_t pair, const char* value);

// установить значение по индексу (true - добавлено или изменено)
bool set_by_idx(char* str, uint16_t size, uint16_t idx, const char* value);

// добавить пару
Pair_t add_pair(char* str, uint16_t size, const char* key, const char* value, bool pgm = false);

// ======================== MISC ========================

// посчитать количество пар
uint16_t count_pairs(const char* str);

// проверить существование пары по ключу
bool count_pairs(const char* str, const char* key, bool pgm = false);

// ======================== REMOVE ========================

// удалить пару по индексу
bool remove_by_idx(char* str, uint16_t idx);

// удалить пару по ключу
bool remove_by_key(char* str, const char* key, bool pgm = false);

// удалить пару по паре
bool remove_by_pair(char* str, Pair_t pair);

}  // namespace PairsChar