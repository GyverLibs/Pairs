#include <Arduino.h>
#include <Pairs.h>
#include <PairsStatic.h>


static const char pstr[] PROGMEM = "progmem text";

void testPair(PairsExt& p) {
    Serial.println("============== START ==============");
    p["key0"] = "cstr";
    p[F("key1")] = F("F string");
    String s("String");
    p[String("key") + 2] = s;
    p["key3"] = sutil::AnyText((const __FlashStringHelper*)pstr);
    p["key4"] = true;
    p["key5"] = 12345;
    p["key6"] = 12345789ull;
    p["key7"] = 3.14;
    p["key8"] = sutil::AnyValue(3.141527, 6);

    Serial.println(p["key0"] == "cstr");
    Serial.println(p["key1"] == "F string");
    Serial.println(p["key2"] == "String");
    Serial.println(p["key3"] == "progmem text");

    Serial.println(p["key4"] == true);
    Serial.println(p["key5"] == 12345);
    Serial.println(p["key6"] == 12345789ull);
    Serial.println(p["key7"] == 3.14);
    Serial.println(p["key8"] == 3.141527);

    p.remove(0);
    p.remove(0);
    p.remove(6);
    p.remove(p.amount() - 1);
    p.remove(p.amount() - 1);
    p.remove("key0");  // test
    p.remove("key1");  // test
    p.remove("key2");  // test
    p.remove("key3");  // test
    p.remove("key4");  // test
    p.remove("key5");  // test
    p.remove("key6");  // test
    p.remove("key7");  // test
    p.remove("key8");  // test
    Serial.println(p.length() == 0);

    // set
    p["key0"] = "val0";
    p["key1"] = "val1";
    p["key2"] = "val2";
    Serial.println(p.asText());

    // change long
    p["key0"] = "val000";
    p["key1"] = "val111";
    p["key2"] = "val222";
    Serial.println(p.asText());

    // change short
    p["key0"] = "v0";
    p["key1"] = "v1";
    p["key2"] = "v2";
    Serial.println(p.asText());

    p.remove("key0");
    p.remove("key1");
    p.remove("key2");
    p.remove(0);               // test
    p.remove(p.amount() - 1);  // test
    Serial.println(p.length() == 0);
}

void setup() {
    Serial.begin(115200);

    char str[200] = {0};
    PairsExt p1(str, 200);
    Pairs p2;
    PairsStatic<200> p3;
    testPair(p1);
    testPair(p2);
    testPair(p3);
    Serial.println("============== DONE ==============");
}
void loop() {
}