#include <Arduino.h>
#include <Pairs.h>
#include <PairsStatic.h>

static const char pstr[] PROGMEM = "progmem text";

struct Data {
    int i;
    float f;
};

template <typename T>
void testPair(T& p) {
    Data data{12345, 3.14};

    Serial.print("\nadd+compare: ");
    p["key0"] = "cstr";
    p[F("key1")] = F("F string");
    String key("key2");
    String val("String");
    p[key] = val;
    p[String("key3")] = String("const String&");
    p["key4"] = sutil::AnyText((const __FlashStringHelper*)pstr);
    p["key5"] = true;
    p["key6"] = 12345;
    p["key7"] = 12345789ull;
    p["key8"] = 3.14;
    p["key9"] = sutil::AnyValue(3.141527, 6);
    p["key10"] = pairs::Value(&data, sizeof(data));

    Serial.print(p["key0"] == "cstr");
    Serial.print(p["key1"] == "F string");
    Serial.print(p["key2"] == "String");
    Serial.print(p["key3"] == "const String&");
    Serial.print(p["key4"] == "progmem text");
    Serial.print(p["key5"] == true);
    Serial.print(p["key6"] == 12345);
    Serial.print(p["key7"] == 12345789ull);
    Serial.print(p["key8"] == 3.14);
    Serial.print(p["key9"] == 3.141527);
    Data data2;
    p["key10"].decodeB64(&data2, sizeof(data2));
    Serial.print(data.f == data2.f);
    Serial.print(data.i == data2.i);

    Serial.print("\r\nremove: ");
    p.remove(0);               // 0
    p.remove(0);               // 1
    p.remove(7);               // 10
    p.remove(p.amount() - 1);  // 9
    p.remove(p.amount() - 1);  // 8
    p.remove("key2");          // test
    p.remove("key3");          // test
    p.remove("key4");          // test
    p.remove("key5");          // test
    p.remove("key6");          // test
    p.remove("key7");          // test
    Serial.print(p.length() == 0);

    p.fromText(F("\"key0\":v0\n\"key1\":v1\n\"key2\":v2"));
    p.remove(0);
    Serial.print(p == F("\"key1\":v1\n\"key2\":v2"));

    p.fromText(F("\"key0\":v0\n\"key1\":v1\n\"key2\":v2"));
    p.remove(1);
    Serial.print(p == F("\"key0\":v0\n\"key2\":v2"));

    p.fromText(F("\"key0\":v0\n\"key1\":v1\n\"key2\":v2"));
    p.remove(2);
    Serial.print(p == F("\"key0\":v0\n\"key1\":v1"));

    p.clear();

    Serial.print("\r\nchange: ");

    // set
    p["key0"] = "val0";
    p["key1"] = "val1";
    p["key2"] = "val2";

    // change long
    p["key0"] = "val000";
    p["key1"] = "val111";
    p["key2"] = "val222";
    Serial.print(p == F("\"key0\":val000\n\"key1\":val111\n\"key2\":val222"));

    // change short
    p["key0"] = "v0";
    p["key1"] = "v1";
    p["key2"] = "v2";
    Serial.print(p == F("\"key0\":v0\n\"key1\":v1\n\"key2\":v2"));
}

void setup() {
    Serial.begin(115200);

    char str[200] = {0};
    PairsExt p1(str, 200);
    PairsStatic<200> p2;
    Pairs p3;
    testPair(p1);
    testPair(p2);
    testPair(p3);

    Serial.println();
    Serial.println("DONE");
}
void loop() {
}