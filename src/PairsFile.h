#pragma once

#include <Arduino.h>

#include "Pairs.h"

#if defined(ESP8266) || defined(ESP32)

#include <FS.h>

class PairsFile : public Pairs {
   public:
    PairsFile(fs::FS* nfs = nullptr, const char* path = nullptr, uint32_t tout = 10000) {
        setFS(nfs, path);
        _tout = tout;
    }

    // установить файловую систему и имя файла
    void setFS(fs::FS* nfs, const char* path) {
        _fs = nfs;
        _path = path;
    }

    // установить таймаут записи, мс (умолч. 10000)
    void setTimeout(uint32_t tout = 10000) {
        _tout = tout;
    }

    // прочитать данные в буфер. Опционально заразервировать дополнительное место. true если прочитаны
    bool begin(uint16_t res = 0) {
        if (!_fs) return 0;
        if (_fs->exists(_path)) {
            File file = _fs->open(_path, "r");
            if (!file) return 0;
            if (!reserve(file.size() + res)) return 0;
            file.read((uint8_t*)_str, file.size());
            ((char*)_str)[file.size()] = '\0';
            refresh();
        } else {
            File file = _fs->open(_path, "w");
            if (!file) return 0;
        }
        return 1;
    }

    // обновить данные в файле сейчас
    bool update() {
        File file = _fs->open(_path, "w");
        if (!file) return 0;
        uint16_t len = file.write((uint8_t*)_str, length());
        return length() == len;
    }

    // тикер, вызывать в loop. Сам обновит данные при изменении и выходе таймаута, вернёт true
    bool tick() {
        if (changed()) {
            _tmr = millis();
            if (!_tmr) _tmr = 1;
        }
        if (_tmr && millis() - _tmr >= _tout) {
            _tmr = 0;
            update();
            return 1;
        }
        return 0;
    }

   private:
    fs::FS* _fs;
    const char* _path;
    uint32_t _tmr = 0, _tout = 10000;
};

#endif