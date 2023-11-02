#include "utils/PairsChar.h"

namespace PairsChar {
// ======================== GET ========================

// получить пару по ключу
Pair_t get_pair_by_key(const char* str, const char* key, bool pgm) {
    Pair_t pair;
    const char* p = str;
    uint16_t len = pgm ? strlen_P(key) : strlen(key);
    pair.key_len = len;

    while (1) {
        if (p[0] != '\"') return Pair_t();
        pair.key = p + 1;
        p = strchr(p + 1, '\"');
        if (!p) return Pair_t();
        if ((p - pair.key == len) &&
            p[1] == ':' &&
            (pgm ? !strncmp_P(pair.key, key, len) : !strncmp(pair.key, key, len))) {
            p++;
            pair.val = p + 1;
            while (1) {
                const char* pt = p;
                p = strchr(p + 1, '\"');
                if (!p) {
                    pair.val_len = strchr(pt + 1, '\0') - pair.val;
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

// получить пару по индексу
Pair_t get_pair_by_idx(const char* str, uint16_t idx) {
    if (str[0] != '\"' || idx >= count_pairs(str)) return Pair_t();
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
                if (!p) pair.val_len = strchr(p, '\0') - pair.val;
                else pair.val_len = p - pair.val - 1;  // -1 for div
                return pair;
            } else break;
        }
        p++;
        i++;
    }
    return Pair_t();
}

// ======================== SET ========================

// установить значение по ключу(true - добавлено или изменено)
bool set_by_key(char* str, uint16_t size, const char* key, const char* value, bool pgm) {
    Pair_t pair;
    if (str[0] == '\"') pair = get_pair_by_key(str, key, pgm);
    if (pair.val) return set_by_pair(str, size, pair, value);
    else return add_pair(str, size, key, value, pgm).val;
}

// установить значение по паре (true - добавлено или изменено)
bool set_by_pair(char* str, uint16_t size, Pair_t pair, const char* value) {
    if (!pair.val) return 0;
    uint16_t vlen = strlen(value);
    if (vlen == pair.val_len && !strncmp(pair.val, value, pair.val_len)) return 0;  // same
    int16_t dif = vlen - pair.val_len;
    uint16_t len = strlen(str);
    if (dif > 0 && len + dif + 1 > size) return 0;              // +1 for \0
    uint16_t left = str + len - (pair.val + pair.val_len) + 1;  // +1 for \0
    memmove((void*)(pair.val + vlen), pair.val + pair.val_len, left);
    memcpy((void*)pair.val, value, vlen);
    return 1;
}

// установить значение по индексу (true - добавлено или изменено)
bool set_by_idx(char* str, uint16_t size, uint16_t idx, const char* value) {
    Pair_t pair = get_pair_by_idx(str, idx);
    if (!pair.val) return 0;
    return set_by_pair(str, size, pair, value);
}

// добавить пару
Pair_t add_pair(char* str, uint16_t size, const char* key, const char* value, bool pgm) {
    Pair_t pair;
    uint16_t len = strlen(str);
    pair.key_len = pgm ? strlen_P(key) : strlen(key);
    pair.val_len = strlen(value);
    if (len + !!len + pair.key_len + pair.val_len + 3 + 1 > size) return Pair_t();  // +1 for \0
    char* p = str + len;
    if (len) *(p++) = '\n';  // += \n
    *(p++) = '\"';
    pair.key = p;
    if (pgm) memcpy_P(p, key, pair.key_len);
    else memcpy(p, key, pair.key_len);
    p += pair.key_len;
    *(p++) = '\"';
    *(p++) = ':';
    pair.val = p;
    memcpy(p, value, pair.val_len);
    p += pair.val_len;
    *p = '\0';
    return pair;
}

// ======================== MISC ========================

// посчитать количество пар
uint16_t count_pairs(const char* str) {
    if (str[0] != '\"') return 0;
    uint16_t i = 1;
    while (1) {
        str = strchr(str + 1, '\"');  // skip 1st
        if (!str) break;
        if (str[-1] != '\\') i++;
    }
    return (i >> 1);  // /2
}

// проверить существование пары по ключу
bool count_pairs(const char* str, const char* key, bool pgm) {
    uint16_t len = pgm ? strlen_P(key) : strlen(key);
    char buf[len + 3 + 1];  // "": == 3
    pgm ? memcpy_P(buf + 1, key, len) : memcpy(buf + 1, key, len);
    buf[len + 1] = buf[0] = '\"';
    buf[len + 2] = ':';
    buf[len + 3] = 0;
    return strstr(str, buf);
}

// ======================== REMOVE ========================

// удалить пару по индексу
bool remove_by_idx(char* str, uint16_t idx) {
    Pair_t pair = get_pair_by_idx(str, idx);
    return remove_by_pair(str, pair);
}

// удалить пару по ключу
bool remove_by_key(char* str, const char* key, bool pgm) {
    Pair_t pair = get_pair_by_key(str, key, pgm);
    return remove_by_pair(str, pair);
}

// удалить пару по паре
bool remove_by_pair(char* str, Pair_t pair) {
    if (!pair.val) return 0;
    bool first = (pair.key - 1) == str;
    bool last = *(pair.val + pair.val_len) == '\0';
    if (last) {
        if (first) *str = '\0';
        else *((char*)pair.key - 2) = '\0';
    } else {
        char* end = strchr(str, '\0');
        char* src = (char*)(pair.val) + pair.val_len;
        if (first) memmove((void*)str, src + 1, end - src);
        else memmove((void*)(pair.key - 1), src + 1, end - src);
    }
    return 1;
}

}  // namespace PairsChar