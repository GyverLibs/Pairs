This is an automatic translation, may be incorrect in some places. See sources and examples!

# Pairs
Storage of any data in text form in the format `" key ": Value`
- Fast and easy implementation compared to json
- Boofer isolation: static or dynamic (like String)
- access (reading and recording) through [] is implemented
- native converting from any type of record, output to the selected type of reading
- A separate tool for ESP8266/ESP32 - automatic recording and reading to the file
- Correct work with memory

## compatibility
Compatible with all arduino platforms (used arduino functions)

## Content
- [installation] (# Install)
- [documentation] (#docs)
- [use] (#usage)
- [versions] (#varsions)
- [bugs and feedback] (#fedback)

<a id="install"> </a>
## Installation
- The library can be found by the name ** pairs ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download the library] (https://github.com/gyverlibs/pairs/archive/refs/heads/main.zip) .Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!


<a id="docs"> </a>
## Documentation

### Data
Between the pairs, the separator `\ n` (the transfer of the line) is placed, after the last pair is not placed.Data type as a text:
`` `
"Key0": Value0
"Key1": value1
"Key2": value2
`` `

During editing through the library methods (add, delete, change), the library itself monitors the correct location of the dividers.

### Shinting
The key and value should not contain unexplored double quotes `` ` - this will break reading and recording! The library does not follow this and the quotation marks do not shield! If a symbol of double quotes are needed in the field - it needs to be shifted by a symbol` \ ``

#### Type of data as a text
`` `
"Key0": Val "ue0 - wrong
"Key1": val \ "ue1 - correctly
`` `

#### Type of Data in a line
`` `CPP
Data ["Key"] = "Val \" ue "; // is wrong (equivalent to the value of val" ue)
Data ["Key"] = "Val \\\" UE "; // Correctly (equivalent to the value of val \" ue)
`` `

When the value is output through `Tostring ()` The shielding is ignored, but you can convey `Tostring (false)` so that the line gets out without ignoring the screens.

### Data types
In the documentation below, the type of data `Anytext` can accept:
- `COST Char*`, `char*`, `" string constants "`
- `f (" lines ")` - linesfrom Progmem
- `string` - lines

Type `Anyvalue` can take:
- Lines like `Anytext`
- All integers (8, 16, 32 bits)
- `Double` and` float`

### Access through []
The library has access to values through squares for reading and writing.This is convenient, but there is a feature: when reading a key that is not in the database, a new field will be created with this key and empty knowledge!When using the function `get ()` this does not happen.

The library contains several classes for working in different scenarios:

### classes
<details>
<summary> pairsext </summary>

The main object of steam based on a static external `char` of the indicated length.

`` `CPP
// Designer
Pairsext ();
Pairsext (char* str, uint16_t size);// connect an external size size buffer

// variables
Char* str;// Line for manual access
uint16_t size;// specified Max.size

// Methods
VOID SetBuffer (Char* str, uint16_t len); // Connect the buffer
Void Clear ();// Clean the line
Bool Changed ();// there was a change in data.Will drop in FALSE itself
Bool Contains (Anytext Key);// existence
uint16_t Length ();// Actual line length
uint16_t amount ();// The number of steam
VOID Refresh ();// count the length of the line and the number of pairs (after manual changes)

Bool Set (Anytext Key, Anyvalue Value);// install on the key
Bool setn (Uint16_t IDX, ANYVALUE VALUE); // Install the index

Pair get (Anytext Key);// get on the key
Pair getn (uint16_t IDX);// Get the index

int32_t toint ();// Bend into int
Float Tofloat ();// Bring to Float
String Tostring ();// Bring to String
Bool Tochar (Char* Buf, Uint16_T LEN);// Bring out in the Char

Bool Remove (Anytext Key);// Delete on the key
Bool Removen (Uint16_T IDX);// Delete the index
`` `
</details>
<details>
<summary> pairs </summary>

Object of steam based on a dynamic line.Methods are the same as `Pairsext`, with the exception of SetBuffer/Reserv.

`` `CPP
// Designer
Pairs ();
Pairs (uint16_t size);// indicating the string reserve

// Methods
Bool Reserve (Uint16_T LEN);// Reserve a line
// inherits everything from PAIRSEXT
`` `
</details>
<details>
<summary> pairsstatic </summary>

Based on `Pairsext`, but instead of an external array, it creates its own, inside the object.

`` `CPP
// Designer
Pairsstatic <Max.size> ();

// Methods
// inherits everything from PAIRSEXT
`` `
</details>
<details>
<summary> pairsfile </summary>

Automatic storage and updating of the pair base for ESP8266/ESP32.It is attached to the file, writes data into it when changing + the time of the timeout.Based on the dynamic class `pairs`.

`` `CPP
// Designer
// install the file system, file name and timut
PAIRSFILE (fs :: fs* nfs = nullptr, constel* path = nullptr, uint32_t tout = 10000);

// Methods
// inherits everything from pairs

// install the file system and file name
VOID setfs (fs :: fs* nfs, const char* Path);

// Install the Takeout of the Records, MS (silence 1000)
VOID settimeout (uint32_t tout = 10000);

// Read the data in the buffer.Optionally expand the additional place.True if you read
Bool Begin (uint16_t res = 0);

// update data in the file
Bool update ();

// ticker, call in LOOP.He will update the data himself when the timuta is changed and output, it will return True
Bool Tick ();
`` `
</details>
<details>
<summary> pair_t </summary>
The object of the pair, stores the pointers to the key and value and their length, and also allows you to display data to the specified type.

`` `CPP
Const Char* Key;// key
uint16_t key_len;// Key length
const chaar* valle;// meaning
uint16_t val_len;// length of the value

// Bring the value to the chamber array
Bool Tochar (Char* Buf, Uint16_T LEN);
int32_t toint ();// Bring value in int
Float Tofloat ();// Bring the valuein Float
String Tostring ();// Bring the value to string
String Tostring (false);// display a value in string without screens \\
`` `
</details>

<a id="usage"> </a>
## Usage
### General information
In different classes, in fact, only initialization differs:

`` `CPP
// PAIRSEXT
Char str [100] = {0};
PAIRSEXT P (str, 100);

// PAIRSEXT
Pairs p;

// PAIRSEXT
PAIRSSTATIC <00> p;

// Pairsfile
Pairsfile P (& Littlefs, "/data.dat");
// Before calling Begin () the file system must be launched!
`` `

In reading-record, everything is Odiakovo, from the previous example the object `p`:

`` `CPP
// Record.Data types in any combinations
p ["key0"] = "val0";
p [f ("key1")] = 1234;
p [0] = f ("abcd");
P.Set ("Key2", 3.14);
P.Setn (0, ("New Val 0"));

// Reading
Serial.println (p ["key0"]);// Auto-Cast in String
int i = p ["key1"]. Toint ();
Float F = P.Get ("Key2"). Tofloat ();

// Deleting
P.remove (F ("Key1"));
P.removen (0);

// Work with String.So - it will also work
String Val = "Value";
P [String ("Key") + 1] = Val;

// But you need to remember that this can create a fragmentation of memory
// If dynamic pairs are used
`` `

### PAIRSEXT
With manual changes in the buffer (the data are copied from somewhere out of the outside), you need to call `.refresh ()` to recall the database!

### Pairsfile
`` `CPP
Pairsfile Data (& Littlefs, "/data.dat", 3000);

VOID setup () {
    Littlefs.Begin ();
    Data.Begin ();// read from the file

    Data ["key"] = "value";// changed
}

VOID loop () {
    Data.tick ();// tick here.Will update itself after the timeout
}
`` `

<a id="versions"> </a>
## versions
- V1.0
- V1.1
  - dynamic string implementation is replaced by its
  - Added the ability to set values from Progmem
  - The library is lightweight and accelerated
  - More security
- V1.2 - Automatic defecting when displaying in string

<a id="feedback"> </a>
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!

When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code