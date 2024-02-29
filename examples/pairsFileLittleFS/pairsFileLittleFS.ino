#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <PairsFile.h>

PairsFile conFile(&LittleFS, "/net.dat", 3000);

void setup() {
    Serial.begin(115200);
    delay(1000);
    LittleFS.begin();
    conFile.begin();  // прочитать из файла
    Serial.print("data:");
    Serial.println(conFile["key"]);
    delay(3000);
    conFile["key"] = "value";  // изменили
}

void loop() {
    conFile.tick();  // тикаем тут. Само обновится после таймаута
}
