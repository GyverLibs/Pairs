#include "utils/PairsString.h"

namespace PairsString {
// ======================== GET ========================

// получить пару по ключу
Pair_t get_pair_by_key(const String& str, const char* key, bool pgm) {
    return PairsChar::get_pair_by_key(str.c_str(), key, pgm);
}

// получить пару по индексу
Pair_t get_pair_by_idx(const String& str, uint16_t idx) {
    return PairsChar::get_pair_by_idx(str.c_str(), idx);
}

// ======================== SET ========================

// установить значение по ключу (true - добавлено или изменено)
bool set_by_key(String& str, const char* key, const char* value, bool pgm) {
    Pair_t pair;
    if (str[0] == '\"') pair = get_pair_by_key(str, key, pgm);
    if (pair.val) return set_by_pair(str, pair, value);
    else {
        pair = add_pair(str, key, value, pgm);
        return (bool)pair.val;
    }
}

// установить значение по паре (true - добавлено или изменено)
bool set_by_pair(String& str, Pair_t pair, const char* value) {
    if (!pair.val) return 0;
    uint16_t vlen = strlen(value);
    if (vlen == pair.val_len && !strncmp(pair.val, value, pair.val_len)) return 0;  // same
    int16_t dif = vlen - pair.val_len;
    uint16_t left = str.end() - (pair.val + pair.val_len);
    if (dif >= 0) {
        const char* p = str.c_str();
        Pair_t buf = pair;
        if (!str.reserve(str.length() + dif)) return 0;
        if (str.c_str() != p) {  // string moved
            pair.val = str.c_str() + (buf.val - p);
            pair.key = str.c_str() + (buf.key - p);
        }
        for (uint16_t i = 0; i < (uint16_t)dif; i++) str += '!';  // dummy
    }
    memmove((void*)(pair.val + vlen), pair.val + pair.val_len, left);
    memcpy((void*)pair.val, value, vlen);
    if (dif < 0) str.remove(str.length() + dif);  // - (-dif)
    return 1;
}

// установить значение по индексу (true - добавлено или изменено)
bool set_by_idx(String& str, uint16_t idx, const char* value) {
    Pair_t pair = get_pair_by_idx(str, idx);
    if (!pair.val) return 0;
    return set_by_pair(str, pair, value);
}

// добавить пару
Pair_t add_pair(String& str, const char* key, const char* value, bool pgm) {
    Pair_t pair;
    if (str.length()) str += '\n';
    pair.key_len = pgm ? strlen_P(key) : strlen(key);
    pair.val_len = strlen(value);
    if (!str.reserve(str.length() + pair.key_len + pair.val_len + 3)) return Pair_t();
    str += '\"';
    pair.key = str.end();
    if (pgm) str += (const __FlashStringHelper*)key;
    else str += key;
    str += '\"';
    str += ':';
    pair.val = str.end();
    str += value;
    return pair;
}

// ======================== MISC ========================

// посчитать количество пар
uint16_t count_pairs(const String& str) {
    return PairsChar::count_pairs(str.c_str());
}

// проверить существование пары по ключу
bool count_pairs(const String& str, const char* key, bool pgm) {
    return PairsChar::count_pairs(str.c_str(), key, pgm);
}

// ======================== REMOVE ========================

// удалить пару по индексу
bool remove_by_idx(String& str, uint16_t idx) {
    Pair_t pair = get_pair_by_idx(str, idx);
    return remove_by_pair(str, pair);
}

// удалить пару по ключу
bool remove_by_key(String& str, const char* key, bool pgm) {
    Pair_t pair = get_pair_by_key(str, key, pgm);
    return remove_by_pair(str, pair);
}

// удалить пару по паре
bool remove_by_pair(String& str, Pair_t pair) {
    if (!pair.val) return 0;
    bool first = (pair.key - 1) == str.c_str();
    bool last = *(pair.val + pair.val_len) == '\0';
    str.remove((pair.key - str.c_str()) - 1 - !first, pair.key_len + pair.val_len + 3 + (!first || !last));
    return 1;
}

/*
  first && last		    "KEY":VAL						  key-1, keyl+vall+3
  !first && last		"key":val;"key":val;"KEY":VAL	  key-2, keyl+vall+4
  first && !last		"KEY":VAL;"key":val;"key":val	  key-1, keyl+vall+4
  !first && !last		"key":val;"KEY":VAL;"key":val	  key-2, keyl+vall+4
*/

}