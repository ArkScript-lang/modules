# ArkScript modules

**[ArkScript](https://github.com/ArkScript-lang/Ark)** main repository

**Warning**: This repository can not be used alone, it was made to separate the modules' code from the source code of the language.

## Making a module

Modules must use the exact same configuration and compiler as the original ArkScript executable, otherwise they won't be loaded properly, thus it's highly encouraged to compile your module(s) using ArkScript CMakeLists, by putting your module(s) under the folder `arkscript/module` (ie, in this repository), and adding it in the CMakeLists of the same folder (you can start by copying the CMakeLists of a module for your own).

The compilers supported are:
* for Linux: g++-8 (Ubuntu 8.3.0-6ubuntu1~18.04.1) 8.3.0
* for Windows: MSVC version 19.16.27031.1 for x86 (*nota bene*: even if minor and patch are be different, it should work)

Basic module code:

```cpp
// this file includes everything you need for you, and does a using namespace Ark
#include <Ark/Module.hpp>

namespace MyModule
{
    Value foo(std::vector<Value>& args)
    {
        // do stuff
        // ...

        return Nil;
    }
}

// the name mustn't be misspelled, otherwise the ArkVM won't find the plugin entry table
ARK_API_EXPORT Mapping_t getFunctionsMapping()
{
    Mapping_t map;
    // map[name in Ark] = C++ function
    map["foo"] = MyModule::foo;
    map["test"] = [](std::vector<Value>& args) {
        // lambdas work too!
        return True;
    };

    return map;
}
```

For more information, see [Embedding](https://github.com/ArkScript-lang/Ark/wiki/Embedding).

## Available modules

*Nota bene*: You can find the documentation of each module under `[module name]/documentation`.

### console

To play with the colors of the console.

### http

Using [cpp-httplib](https://github.com/yhirose/cpp-httplib) under MIT licence.

To create web servers and make http requests.

### msgpack

Using [msgpack-c](https://github.com/msgpack/msgpack-c) under Boost Software Licence version 1.0.

To pack and unpack data into/from MessagePack format.

### random

To generate random numbers using a Mersenne Twister 19937.

### sfml

Using [SFML](https://github.com/SFML/SFML) under MIT licence (projects used by the SFML are all compatible with the MPL v2.0).

To create SFML windows, load and display images, fonts, play sounds...

## Dependencies

* C++17
* CMake >= 3.12
* Visual Studio >= 11 (on Windows)

## Copyright and Licence information

Copyright (c) 2019 Alexandre Plateau. All rights reserved.

This ArkScript distribution contains no GNU GPL code, which means it can be used in proprietary projects.