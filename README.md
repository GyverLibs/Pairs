[![latest](https://img.shields.io/github/v/release/GyverLibs/Pairs.svg?color=brightgreen)](https://github.com/GyverLibs/Pairs/releases/latest/download/Pairs.zip)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/Pairs?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# Pairs
Хранение любых данных в текстовом виде в формате "ключ":значение
- Быстрая и лёгкая реализация по сравнению с JSON
- Выделение буфера: статическое или на базе String
- Реализован доступ (чтение и запись) через []
- Нативная конвертация из любых типов на запись, вывод в выбранный тип на чтение
- Отдельный инструмент для esp8266/esp32 - автоматическая запись и чтение в файл
- Аккуратная работа с памятью

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Документация](#docs)
- [Использование](#usage)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **Pairs** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/Pairs/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!


<a id="docs"></a>
## Документация
Библиотека содержит в себе несколько классов для работы в разных сценариях.

### Примечание по типам данных
В документации ниже тип данных `AnyText` может принимать:
- `"строки"` - const char*, char*, строковые константы
- `F("строки")` - строки из PROGMEM
- `String` - строки

Тип `AnyValue` может принимать:
- Все целые числа (8, 16, 32 бит)
- `double`, `float`
- `String` и `const char*` строки

### Основные
<details>
<summary>Pairs</summary>
Объект пар на основе динамической String строки.

```cpp
// конструктор
Pairs();
Pairs(uint16_t size);                   // с указанием резерва строки
Pairs(String& str);                     // из строки
Pairs(String& str, uint16_t size);      // из строки + резерв

// переменные
String str;                             // строка для ручного доступа

// методы
bool reserve(uint16_t len);             // зарезервировать строку
void clear();                           // очистить строку
bool changed();                         // было изменение данных. Само сбросится в false
void forceChange();                     // установить флаг на изменение

bool set(AnyText key, AnyValue value);  // установить по ключу
bool setN(uint16_t idx, AnyValue value);// установить по индексу

Pair_t get(AnyText key);                // получить по ключу
Pair_t getN(uint16_t idx);              // получить по индексу

int32_t toInt();                        // вывести в int
float toFloat();                        // вывести в float
String toString();                      // вывести в String
bool toChar(char* buf, uint16_t len);   // вывести в char массив

bool remove(AnyText key);               // удалить по ключу
bool removeN(uint16_t idx);             // удалить по индексу

bool contains(AnyText key);             // проверка на существование
uint16_t length();                      // получить количество пар
```
</details>
<details>
<summary>PairsExt</summary>
Объект пар на основе статического внешнего `char` массива указанной длины. Методы такие же как у Pairs, за исключением setBuffer/reserve.

```cpp
// конструктор
PairsExt();
PairsExt(char* str, uint16_t size);     // подключить внешний буфер размера size

// переменные
char* str;                              // строка для ручного доступа
uint16_t size;                          // указанный макс. размер

// методы
void setBuffer(char* str, uint16_t len);// подключить буфер
void clear();                           // очистить строку
bool changed();                         // было изменение данных. Само сбросится в false
void forceChange();                     // установить флаг на изменение

bool set(AnyText key, AnyValue value);  // установить по ключу
bool setN(uint16_t idx, AnyValue value);// установить по индексу

Pair_t get(AnyText key);                // получить по ключу
Pair_t getN(uint16_t idx);              // получить по индексу

int32_t toInt();                        // вывести в int
float toFloat();                        // вывести в float
String toString();                      // вывести в String
bool toChar(char* buf, uint16_t len);   // вывести в char массив

bool remove(AnyText key);               // удалить по ключу
bool removeN(uint16_t idx);             // удалить по индексу

bool contains(AnyText key);             // проверка на существование
uint16_t length();                      // получить количество пар
```
</details>
<details>
<summary>PairsStatic</summary>
Основан на `PairsExt`, но вместо внешнего массива создаёт свой, внутри объекта.

```cpp
// конструктор
PairsStatic<макс. размер> ();

// наследует всё из PairsExt
```
</details>
<details>
<summary>PairsFile</summary>
Автоматическое хранение и обновление базы пар для esp8266/esp32. Привязывается к файлу, записывает в него данные при изменении + выходе таймаута. Основано на классе `Pairs`, т.е. на динамической строке `String`.

```cpp
// наследует всё из Pairs

// конструктор. Установить файловую систему, имя файла и таймаут
PairsFile(fs::FS* nfs = nullptr, const char* path = nullptr, uint32_t tout = 10000);

// установить файловую систему и имя файла
void setFS(fs::FS* nfs, const char* path);

// установить таймаут записи, мс (умолч. 10000)
void setTimeout(uint32_t tout = 10000);

// прочитать данные в буфер. Опционально заразервировать дополнительное место. true если прочитаны
bool begin(uint16_t res = 0);

// обновить данные в файле
bool update();

// тикер, вызывать в loop. Сам обновит данные при изменении и выходе таймаута, вернёт true
bool tick();
```
</details>

### Вспомогательные
<details>
<summary>Pair_t</summary>
Объект пары, хранит указатели на ключ и значение и их длину.

```cpp
const char* key;    // ключ
uint16_t key_len;   // длина ключа
const char* val;    // значение
uint16_t val_len;   // длина значения

// вывести значение в char массив
bool toChar(char* buf, uint16_t len);
int32_t toInt();    // вывести значение в int
float toFloat();    // вывести значение в float
String toString();  // вывести значение в String
```
</details>
<details>
<summary>PairsChar</summary>
Набор функций для работы с парами, текст хранится в char массиве.

```cpp
// получить пару по ключу
Pair_t get_pair_by_key(const char* str, const char* key, bool pgm = false);

// получить пару по индексу
Pair_t get_pair_by_idx(const char* str, uint16_t idx);

// установить значение по ключу(true - добавлено или изменено)
bool set_by_key(char* str, uint16_t size, const char* key, const char* value, bool pgm = false);

// установить значение по паре (true - добавлено или изменено)
bool set_by_pair(char* str, uint16_t size, Pair_t pair, const char* value);

// установить значение по индексу (true - добавлено или изменено)
bool set_by_idx(char* str, uint16_t size, uint16_t idx, const char* value);

// добавить пару
Pair_t add_pair(char* str, uint16_t size, const char* key, const char* value, bool pgm = false);

// посчитать количество пар
uint16_t count_pairs(const char* str);

// проверить существование пары по ключу
bool count_pairs(const char* str, const char* key, bool pgm = false);

// удалить пару по индексу
bool remove_by_idx(char* str, uint16_t idx);

// удалить пару по ключу
bool remove_by_key(char* str, const char* key, bool pgm = false);

// удалить пару по паре
bool remove_by_pair(char* str, Pair_t pair);
```
</details>
<details>
<summary>PairsString</summary>
Набор функций для работы с парами, текст хранится в String строке.

```cpp
// получить пару по ключу
Pair_t get_pair_by_key(const String& str, const char* key, bool pgm = false);

// получить пару по индексу
Pair_t get_pair_by_idx(const String& str, uint16_t idx);

// установить значение по ключу (true - добавлено или изменено)
bool set_by_key(String& str, const char* key, const char* value, bool pgm = false);

// установить значение по паре (true - добавлено или изменено)
bool set_by_pair(String& str, Pair_t pair, const char* value);

// установить значение по индексу (true - добавлено или изменено)
bool set_by_idx(String& str, uint16_t idx, const char* value);

// добавить пару
Pair_t add_pair(String& str, const char* key, const char* value, bool pgm = false);

// посчитать количество пар
uint16_t count_pairs(const String& str);

// проверить существование пары по ключу
bool count_pairs(const String& str, const char* key, bool pgm = false);

// удалить пару по индексу
bool remove_by_idx(String& str, uint16_t idx);

// удалить пару по ключу
bool remove_by_key(String& str, const char* key, bool pgm = false);

// удалить пару по паре
bool remove_by_pair(String& str, Pair_t pair);
```
</details>

<a id="usage"></a>
## Использование
У разных классов по сути отличается только инициализация:

```cpp
// PairsExt
char str[100] = {0};
PairsExt p(str, 100);

// PairsExt
Pairs p;

// PairsExt
PairsStatic<100> p;

// PairsFile
PairsFile p(&LittleFS, "/data.dat");
// перед вызовом begin() файловая система должна быть запущена!
```

В чтении-записи всё одиаково, из предыдущего примера объект `p`:

```cpp
// запись. Типы данных в любых сочетаниях
p["key0"] = "val0";
p[F("key1")] = 1234;
p.set("key2", 3.14);
p.setN(0, ("new val 0"));

// чтение
Serial.println(p["key0"]);  // авто-каст в String
int i = p["key1"].toInt();
float f = p.get("key2").toFloat();

// удаление
p.remove(F("key1"));
p.removeN(0);

// работа со String. Вот так - тоже будет работать
String val = "value";
p[String("key") + 1] = val;

// но нужно помнить, что это может создавать фрагментацию памяти
// если используется Pairs (на базе String)
```

### PairsFile
```cpp
PairsFile data(&LittleFS, "/data.dat", 3000);

void setup() {
    LittleFS.begin();
    data.begin();   // прочитать из файла

    data["key"] = "value";  // изменили
}

void loop() {
    data.tick();    // тикаем тут. Само обновится после таймаута
}
```

<a id="versions"></a>
## Версии
- v1.0

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код