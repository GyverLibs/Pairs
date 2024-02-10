#include <Arduino.h>
#include <Pairs.h>
#include <PairsExt.h>
#include <PairsStatic.h>

void setup() {
  Serial.begin(115200);

  // return;

  // TEST
  char str[100] = {0};
  PairsExt p(str, 100);

  // Pairs p;

  // PairsStatic<100> p;

  // set
  p["key0"] = "val0";
  p["key1"] = "val1";
  p["key2"] = "val2";

  // change long
  p["key0"] = "val000";
  p["key1"] = "val111";
  p["key2"] = "val222";

  // change short
  p["key0"] = "val0";
  p["key1"] = "val1";
  p["key2"] = "val2";

  // add
  p["key4"] = "val4";
  p["key5"] = "val5";
  p["key6"] = "val6";

  Serial.println(p["key1"]);

  p.removeN(0);
  p.removeN(10);
  p.remove("key1");
  p.remove("key4");
  p.remove("key6");

  Serial.println(p);
  Serial.println("DONE");

  /*
    val1
    "key2":val2
    "key5":val5
  */
  // TEST
}

void loop() {
}