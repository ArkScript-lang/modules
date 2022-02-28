@page hash_module Hash module

## hash:sha256

Generates a SHA256 of a given string.

**Parameters**
- `data`: a string to hash

**Return value** `String`: hexadecimal hash

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
~~~~{.lisp}
(import "hash.arkm")
(print (hash:sha256 "arkscript"))  # c56d0bb03e9e03e7000f7b596cdb333c315523af7e4781f02610c690441c43dd
~~~~

## hash:md5

Generates a MD5 of a given string.

**Parameters**
- `data`: a string to hash

**Return value** `String`: hexadecimal hash

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
~~~~{.lisp}
(import "hash.arkm")
(print (hash:md5 "arkscript"))  # a27ee01c49b7552b8d69a631b7bb23c9
~~~~
