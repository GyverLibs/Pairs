[![latest](https://img.shields.io/github/v/release/GyverLibs/Pairs.svg?color=brightgreen)](https://github.com/GyverLibs/Pairs/releases/latest/download/Pairs.zip)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/Pairs?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# Pairs
Хранение любых данных в текстовом виде в формате `"ключ":значение`
- Быстрая и лёгкая реализация по сравнению с JSON
- Выделение буфера: статическое или динамическое (как у String)
- Доступ (чтение и запись) через []
- Нативная конвертация из любых типов на запись, вывод в выбранный тип на чтение
- Отдельный инструмент для esp8266/esp32 - автоматическая запись и чтение в файл

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Документация](#docs)
- [Использование](#usage)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="docs"></a>
## Документация

Библиотека содержит в себе несколько классов для работы в разных сценариях:

### Классы
<details>
<summary>PairsExt</summary>

Основной объект пар на основе статического внешнего `char` массива указанной длины.

```cpp
// конструктор
PairsExt();
PairsExt(char* str, uint16_t size);     // подключить внешний буфер размера size

// методы
uint16_t getSize();                     // получить размер буфера
void setBuffer(char* str, uint16_t len);// подключить буфер
void clear();                           // очистить строку
bool changed();                         // было изменение данных. Само сбросится в false
bool contains(AnyText key);             // проверка на существование
uint16_t amount();                      // количество пар
void refresh();                         // пересчитать длину строки и количество пар (после ручных изменений)
bool fromText(AnyText text);            // создать из текста

bool set(AnyText key, Value value);             // установить по ключу
bool set(AnyText key, void* var, size_t size);  // установить по ключу

bool set(int idx, Value value);                 // установить по индексу
bool set(int idx, void* var, size_t size);      // установить по индексу

Pair get(AnyText key);                  // получить по ключу
Pair get(int idx);                      // получить по индексу

bool remove(AnyText key);               // удалить по ключу
bool remove(int idx);                   // удалить по индексу
```
</details>
<details>
<summary>Pairs</summary>

Объект пар на основе динамической строки. Методы такие же как у `PairsExt`, за исключением setBuffer/reserve.

```cpp
// конструктор
Pairs();
Pairs(uint16_t size);           // с указанием резерва строки

// методы
bool reserve(uint16_t len);     // зарезервировать строку
// + наследует всё из PairsExt
```
</details>
<details>
<summary>PairsStatic</summary>

Основан на `PairsExt`, но вместо внешнего массива создаёт свой, внутри объекта.

```cpp
// конструктор
PairsStatic<макс. размер> ();

// методы
// + наследует всё из PairsExt
```
</details>
<details>
<summary>PairsFile</summary>

Автоматическое хранение и обновление базы пар для esp8266/esp32. Привязывается к файлу, записывает в него данные при изменении + выходе таймаута. Основано на динамическом классе `Pairs`.

```cpp
// конструктор
// Установить файловую систему, имя файла и таймаут
PairsFile(fs::FS* nfs = nullptr, const char* path = nullptr, uint32_t tout = 10000);

// методы
// наследует всё из Pairs

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
<details>
<summary>Pair</summary>
Объект пары, наследует AnyText для значения и содержит поле AnyText key для ключа.

```cpp
AnyText key;
```
</details>

<a id="usage"></a>

## Использование

### Вид данных
Данные хранятся в текстовом виде в формате пар `"ключ":значение`, между парами ставится разделитель `\n` (перенос строки), после последней пары не ставится. Вид данных как текст:
```
"key0":value0
"key1":value1
"key2":value2
```

Во время редактирования через методы библиотеки (добавить, удалить, изменить) библиотека сама следит за корректным расположением разделителей.

### Доступ через []
В библиотеке реализован доступ к значениям через квадратные скобки на чтение и запись. Это удобно, но есть особенность: при чтении по ключу, которого нет в базе - будет создано новое поле с этим ключом и пустым значением! При использовании функции `get()` этого не происходит. Например в базе нет ключа `key`:
```cpp
Serial.println(data.get("key"));  // ничего не произойдёт
Serial.println(data["key"]);      // будет создан ключ key с пустым значением
```

### Экранирование
Ключ и значение не должны содержать неэкранированных двойных кавычек `"` - это сломает чтение и запись! Библиотека не следит за этим и сама кавычки не экранирует. Если в поле значения нужен символ двойных кавычек - его нужно экранировать символом `\`:

```
"key0":val"ue0  - неправильно
"key1":val\"ue1 - правильно
```
```cpp
data["key"] = "val\"ue";    // неправильно (равносильно значению val"ue)
data["key"] = "val\\\"ue";  // правильно (равносильно значению val\"ue)
```

### AnyText/AnyValue
`Pairs` в некоторых местах наследует инструменты из библиотеки [StringUtils](https://github.com/GyverLibs/StringUtils), полное описание смотри [в документации](https://github.com/GyverLibs/StringUtils?tab=readme-ov-file#anytext). Например любой вызов `data[key]` или `data.get(key)` конвертируется в тип `AnyText`:

#### На ввод
Тип `AnyText` может принимать:
- `const char*`, `char*`, `"строковые константы"`
- `F("строки")` - строки из PROGMEM
- `String` - строки

Тип `Value` может принимать:
- Строки как в `AnyText`
- Все целые числа (8, 16, 32, 64 бит)
- `double` и `float`
- Бинарные данные (base64)

```cpp
data["key0"] = "val0";
data[F("key1")] = 1234;
data["key2"] = 3.14;
data[0] = F("abcd");

data.set("key0", "val0");
data.set("key1", 1234567ul);
```

#### На вывод
`AnyText` выводится в Print, автоматически сравнивается и конвертируется в любые типы данных, что позволяет получать значения напрямую:
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
// итд
```

Сам объект Pairs также является экземпляром `AnyText`, то есть всю базу пар можно:
```cpp
Serial.println(data);   // вывести в Print
String s = data;        // скопировать в String
data1 == data2;         // сравнить с другой базой (как строку)
```

### Бинарные данные
Помимо данных в виде читаемого текста (числа, строки), библиотека позволяет хранить под ключом также сложные данные, например структуры и массивы. Они будут конвертированы в формат base64 и сохранены в виде текста, а при помощи `AnyText::decodeB64` их можно распаковать и записать обратно:

```cpp
struct S {
    float f;
    int i;
};

// запись
S s{3.14, 1234};
int arr[] = {1, 2, 3, 4, 5};

data["key1"] = pairs::Value(&s, sizeof(s));
data.set("key2", &arr, sizeof(arr));

// чтение
S s1;
int arr1[5];

data["key1"].decodeB64(&s1, sizeof(s1));
data.get("key2").decodeB64(&arr1, sizeof(arr1));
```

### Общая информация
У разных классов по сути отличается только инициализация:

```cpp
char str[100] = {0};
PairsExt data(str, 100);

Pairs data;

PairsStatic<100> data;

PairsFile data(&LittleFS, "/data.dat");
data.begin();
// перед вызовом begin() файловая система должна быть запущена!
```

### PairsExt
При ручных изменениях в буфере (данные скопированы откуда-то извне) нужно вызвать `.refresh()` для пересчёта базы данных!

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
- v1.1 
  - Динамическая String реализация заменена на свою
  - Добавлена возможность задавать значения из PROGMEM
  - Библиотека облегчена и ускорена
  - Больше безопасности
- v1.2 - автоматическое разэкранирование при выводе в String
- v1.3 - добавлена зависимость от StringUtils. Оптимизация и увеличение возможностей
- v1.3.1 - оптимизация под StringUtils 1.3
- v1.4 - оптимизация, больше интеграции со StringUtils, хранение бинарных данных

<a id="install"></a>
## Установка
- Требуется установленная библиотека [StringUtils](https://github.com/GyverLibs/StringUtils)
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
