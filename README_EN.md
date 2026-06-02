This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/Pairs.svg?color=brightgreen)](https://github.com/GyverLibs/Pairs/releases/latest/download/Pairs.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/Pairs.svg)](https://registry.platformio.org/libraries/gyverlibs/Pairs)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/Pairs?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# Pairs
Storage of any data in text form in format`"ключ":значение`
- Fast and easy implementation compared to JSON
- Buffer allocation: static or dynamic (like String)
- Access (reading and writing) via []
- Native conversion from any type to write, output to the selected type for reading
- Separate tool for esp8266/esp32 - Automatic writing and reading to file

### Compatibility
Compatible with all Arduino platforms (Arduino features are used)

## Contents
- [Installation](#install)
- [Documentation.](#docs)
- [Use of use](#usage)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="docs"></a>
## Documentation.

The library contains several classes to work in different scenarios:

### Classes
<details>
<summary>PairsExt</summary>

The main object of pairs based on a static external`char`an array of specified length.

```cpp
// designer
PairsExt();
PairsExt(char* str, uint16_t size);     // Connect an external size buffer

// method
uint16_t getSize();                     // buffer
void setBuffer(char* str, uint16_t len);// plug in
void clear();                           // clear out
bool changed();                         // There was a data change. He'll throw himself into falsehood.
bool has(Text key);                     // test
uint16_t amount();                      // pairing
void refresh();                         // Recalculate the length of the line and the number of pairs (after manual changes)
bool fromText(Text text);               // text

bool set(Text key, Value value);             // key
bool set(Text key, void* var, size_t size);  // key

bool set(int idx, Value value);             // index
bool set(int idx, void* var, size_t size);  // index

Pair get(Text key);                     // key out
Pair get(int idx);                      // index

bool remove(Text key);                  // key out
bool remove(int idx);                   // index
```
</details>
<details>
<summary>Pairs</summary>

A pair object based on a dynamic string. The methods are the same as`PairsExt`excluding setBuffer/reserve.

```cpp
// designer
Pairs();
Pairs(uint16_t size);           // line reserve

// method
bool reserve(uint16_t len);     // reserve

// copy
bool copy(const Pairs& val);

// transfer from another copy
void move(Pairs& rval);

// memory
void reset();

// + inherits everything from PairsExt
```
</details>
<details>
<summary>PairsStatic</summary>

Based on`PairsExt`But instead of an external array, it creates its own, inside the object.

```cpp
// designer
PairsStatic<макс. размер> ();

// method
// + inherits everything from PairsExt
```
</details>
<details>
<summary>PairsFile</summary>

Automatic storage and updating of the pair base for esp8266/esp32. It is tied to the file, writes data to it when changing + timeout output. Based on dynamic class`Pairs`.

```cpp
// designer
// Install the file system, file name and timeout
PairsFile(fs::FS* nfs = nullptr, const char* path = nullptr, uint32_t tout = 10000);

// method
// Inherit everything from Pairs

// install the file system and file name
void setFS(fs::FS* nfs, const char* path);

// Set timeout records, MS (silent 10000)
void setTimeout(uint32_t tout = 10000);

// read the data in the buffer. Optionally infect an additional place. True if you read it.
bool begin(uint16_t res = 0);

// update the file
bool update();

// ticker, call the loop. You will update the data when you change and exit the timeout, return the true
bool tick();
```</details >>
<details >>
<summary>Pair</summary >>
The pair object inherits the Text for the value and contains the Text key field for the key.

```cpp
Text key;
```
</details>

<a id="usage"></a>

## Use of use

### Type of data
Data is stored in text form in pair format`"ключ":значение`There is a divider between pairs.`\n`(Transfer line), after the last pair is not put. Data view as text:
```
"key0":value0
"key1":value1
"key2":value2
```

During editing through library methods (add, delete, modify), the library itself monitors the correct location of the separators.

### Access via []
The library has access to values through square brackets for reading and writing. This is convenient, but there is a feature: when reading on a key that is not in the database, a new field with this key and an empty value will be created! When using a function`get()`That's not happening. For example, there's no key in the database.`key`:
```cpp
Serial.println(data.get("key"));  // Nothing will happen.
Serial.println(data["key"]);      // A key with an empty value will be created. n
```

### Screening
The key and value should not contain unshielded double quotation marks`"`- it'll break reading and writing! The library does not monitor this and the quotes themselves do not screen. If a double quotation marks symbol is needed in the value field, it must be screened with a symbol`\`:

```
"key0":val"ue0  - неправильно
"key1":val\"ue1 - правильно
```
```cpp
data["key"] = "val\"ue";    // incorrectly (equal to val"ue)
data["key"] = "val\\\"ue";  // Right (equal to the value of val\"ue")
```

### Text/Value
`Pairs`in some places inherits tools from the library[StringUtils](https://github.com/GyverLibs/StringUtils)See the full description.[documentation](https://github.com/GyverLibs/StringUtils?tab=readme-ov-file#anytext). For example, any challenge`data[key]`or`data.get(key)`convertible`Text`:

#### On entry.
Type`Text`may take:
- `const char*`, `char*`, `"строковые константы"`
- `F("строки")`- lines from PROGMEM
- `String`- lines

Type`Value`may take:
- Lines like in`Text`
- All integers (8, 16, 32, 64 bits)
- `double`and`float`
- Binary data (base64)

```cpp
data["key0"] = "val0";
data[F("key1")] = 1234;
data["key2"] = 3.14;
data[0] = F("abcd");

data.set("key0", "val0");
data.set("key1", 1234567ul);
```

#### To conclude
`Text`It is displayed in Print, automatically compared and converted to any type of data, which allows you to obtain values directly:
```cpp
data["key"] == 12345;

int i = data["key"];
float f = data["key"];
String s = data["key"];
char buf[20];
data["key"].toStr(buf);

data["key"] == data.get("key2");

Serial.println(data["key"]);
Serial.println(data.get("key"));
// etch
```

The Pairs object itself is also an instance.`Text`That is, the entire base of pairs can:
```cpp
Serial.println(data);   // print out
String s = data;        // string
data1 == data2;         // Compare it to another base (like a string)
```

### Binary data
In addition to data in the form of readable text (numbers, lines), the library allows you to store complex data, such as structures and arrays, on a turnkey basis. They will be converted to base64 format and saved as text.`Text::decodeB64`They can be unpacked and written back:

```cpp
struct S {
    float f;
    int i;
};

// record
S s{3.14, 1234};
int arr[] = {1, 2, 3, 4, 5};

data["key1"] = pairs::Value(&s, sizeof(s));  // packaging, method 1
data.set("key2", &arr, sizeof(arr));         // package, method 2

// reading
S s1;
int arr1[5];

// unpack
data["key1"].decodeB64(&s1, sizeof(s1));
data.get("key2").decodeB64(&arr1, sizeof(arr1));
```

### General information
In different classes, only initialization differs:

```cpp
char str[100] = {0};
PairsExt data(str, 100);

Pairs data;

PairsStatic<100> data;

PairsFile data(&LittleFS, "/data.dat");
data.begin();
// Before the start() call, the file system must be started!
```

### PairsExt
When manual changes in the buffer (data copied from somewhere outside) you need to call`.refresh()`To count the database!

### PairsFile
```cpp
PairsFile data(&LittleFS, "/data.dat", 3000);

void setup() {
    LittleFS.begin();
    data.begin();   // file-read

    data["key"] = "value";  // changed
}

void loop() {
    data.tick();    // Tick here. It'll update itself after timeout.
}
```

<a id="versions"></a>
## Versions
- v1.0
- v1.1 
  - Dynamic String implementation replaced by its own
  - Added the ability to set values from PROGMEM
  - Library lightened and accelerated
  - More security
- v1.2 - Automatic descreening when output in String
- v1.3 - Added dependence on StringUtils. Optimizing and increasing opportunities
- v1.3.1 - optimization for StringUtils 1. 3
- v1.4 - Optimization, more integration with StringUtils, storage of binary data

<a id="install"></a>
## Installation
- An established library is required[StringUtils](https://github.com/GyverLibs/StringUtils)
- The library can be found by the name **Pairs** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/Pairs/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="feedback"></a>
## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
