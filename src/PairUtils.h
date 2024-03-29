#pragma once
#include <Arduino.h>
#include <StringUtils.h>

#include "Pair.h"

namespace pairs {

class Value : public su::Value {
   public:
    using su::Value::Value;

    Value(void* var, size_t size) : _var(var), _size(size) {
        _len = su::b64::encodedLen(size);
    }

    bool valid() const {
        return su::Value::valid() || _var;
    }

    uint16_t length() const {
        if (_var) {
            return _len;
        } else {
            return su::Value::length();
        }
    }

    bool compare(const Value& value) const {
        if (_var) {
            return 0;  // force
        } else {
            return su::Value::compare(value);
        }
    }

    uint16_t toStr(char* buf, int16_t bufsize = -1, bool terminate = true) const {
        if (_var) {
            return su::b64::encode(buf, (uint8_t*)_var, _size);
        } else {
            return su::Value::toStr(buf, bufsize, terminate);
        }
    }

    void* _var = nullptr;
    size_t _size = 0;
};

typedef void (*SetHook)(void* pairs, const Pair& pair, const pairs::Value& value);

class PairAccess : public su::Text {
   public:
    PairAccess(void* pairs, SetHook hook, const Pair& pair) : su::Text(pair._str, pair._len), pairs(pairs), hook(hook), pair(pair) {}

    void operator=(const pairs::Value& value) {
        if (pair.key.valid() && value.valid()) hook(pairs, pair, value);
    }

    void* pairs;
    SetHook hook;
    Pair pair;
};

}  // namespace pairs