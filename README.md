# ArkScript modules

Main repository: **[ArkScript](https://github.com/ArkScript-lang/Ark)**

**Warning**: This repository can not be used alone, it was made to separate the modules' code from the source code of the language.

## Included modules

## Create new modules
* Run the following command
	`python shell/createmodules/create.py <module-name>`
	This creates a module with the structure configured in `shell/module_structure.json` (This structure can be customised)

### Terminal related

* `console` to change text color when using `print`

### Generic utilities

* `bitwise` for bitwise operations
* `hash` to compute md5 and sha256 hashes

### Data related

* `json` to manipulate and create JSON objects
* `database` to manipulate sqlite database

### Network related

* `http` to make web requests (get, post, put, patch, delete)

## Dependencies

* C++20
* CMake >= 3.8
* Visual Studio >= 11 (on Windows)
* Python >= 2.7

## Copyright and Licence information

Copyright (c) 2019-2025 Lex Plateau. All rights reserved.

This ArkScript distribution contains no GNU GPL code, which means it can be used in proprietary projects.
