#include <Arduino.h>
#include <Pairs.h>
#include <PairsExt.h>
#include <PairsStatic.h>

static const char pstr[] PROGMEM = "progmem text";

void setup() {
    Serial.begin(115200);
    Serial.println();

    // char str[200] = {0};
    // PairsExt p(str, 200);

    Pairs p;
    // PairsStatic<200> p;

    // создать из любого типа
    p["key0"] = "string literal";
    p[F("key1")] = F("F string");
    String s("String");
    p[String("key") + 2] = s;
    p["key3"] = sutil::AnyText((const __FlashStringHelper*)pstr);
    p["key4"] = true;
    p["key5"] = 12345;
    p["key6"] = 12345789ull;
    p["key7"] = 3.14;
    p["key8"] = sutil::AnyValue(3.141527, 6);
    Serial.println(p);

    Serial.println("==========");

    // вывести в любой тип
    char buf[20];
    p["key2"].toStr(buf);
    String ss = p["key3"];
    bool b = p["key4"];
    int i = p["key5"];
    uint64_t u64 = p["key6"];
    float f = p["key7"];
    double d = p["key8"];

    Serial.println(buf);
    Serial.println(ss);
    Serial.println(b);
    Serial.println(i);
    // Serial.println(u64);
    Serial.println(f);
    Serial.println(d, 6);

    Serial.println("==========");

    // удаление
    p.remove("key0");
    p.remove(F("key1"));
    p.remove(String("key") + 2);
    p.removeN(0);  // 3
    p.removeN(0);  // 4
    p.removeN(0);  // 5
    p.removeN(p.amount() - 1);  // 8
    Serial.println(p);

    // ожидаемый вывод
    /*
    "key0":string literal
    "key1":F string
    "key2":String
    "key3":progmem text
    "key4":1
    "key5":12345
    "key6":12345789
    "key7":3.14
    "key8":3.141527
    ==========
    String
    progmem text
    1
    12345
    12345789
    3.14
    3.141527
    ==========
    "key6":12345789
    "key7":3.14
    */
}

void loop() {
}