#pragma once
#include <Arduino.h>

#include "utils/PairsChar.h"

namespace PairsString {
// ======================== GET ========================

// получить пару по ключу
Pair_t get_pair_by_key(const String& str, const char* key, bool pgm = false);

// получить пару по индексу
Pair_t get_pair_by_idx(const String& str, uint16_t idx);

// ======================== SET ========================

// установить значение по ключу (true - добавлено или изменено)
bool set_by_key(String& str, const char* key, const char* value, bool pgm = false);

// установить значение по паре (true - добавлено или изменено)
bool set_by_pair(String& str, Pair_t pair, const char* value);

// установить значение по индексу (true - добавлено или изменено)
bool set_by_idx(String& str, uint16_t idx, const char* value);

// добавить пару
Pair_t add_pair(String& str, const char* key, const char* value, bool pgm = false);

// ======================== MISC ========================

// посчитать количество пар
uint16_t count_pairs(const String& str);

// проверить существование пары по ключу
bool count_pairs(const String& str, const char* key, bool pgm = false);

// ======================== REMOVE ========================

// удалить пару по индексу
bool remove_by_idx(String& str, uint16_t idx);

// удалить пару по ключу
bool remove_by_key(String& str, const char* key, bool pgm = false);

// удалить пару по паре
bool remove_by_pair(String& str, Pair_t pair);

}  // namespace PairsString