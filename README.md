# ArkScript modules

Main repository: **[ArkScript](https://github.com/ArkScript-lang/Ark)**

**Warning**: This repository can not be used alone, it was made to separate the modules' code from the source code of the language.

## Included modules

## Create new modules
* Run the following command
	`sh create-module.sh <module-name>`
	This creates a module with the structure configured in `shell/module_structure.json` (This structure can be customised)

### Terminal related

* `console` to change text color when using `print`

### Data related

* `json` to manipulate and create JSON objects
* `msgpack` to manipulate and create MsgPack objects

### Network related

* `http` to create http clients and servers

## Dependencies

* C++17
* CMake >= 3.12
* Visual Studio >= 11 (on Windows)
* Python >= 2.7

## Copyright and Licence information

Copyright (c) 2019-2021 Alexandre Plateau. All rights reserved.

This ArkScript distribution contains no GNU GPL code, which means it can be used in proprietary projects.