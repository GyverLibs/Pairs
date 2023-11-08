#include <LittleFS.h>
#include "Pairs.h"
#include "PairsFile.h"

PairsFile conFile(&LittleFS, "/net.dat", 3000);

void setup() {
    LittleFS.begin();
    conFile.begin();   // прочитать из файла
    conFile["key"] = "value";  // изменили
}

void loop() {
    conFile.tick();    // тикаем тут. Само обновится после таймаута
}
