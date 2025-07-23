---
title: "JSON module"
description: ""
summary: ""
date: 2025-07-23T14:25:16+02:00
lastmod: 2025-07-23T14:25:16+02:00
draft: false
weight: 410
toc: true
seo:
  title: "" # custom title (optional)
  description: "" # custom description (recommended)
  canonical: "" # custom canonical URL (optional)
  noindex: false # false (default) or true
---

{{< highlight_scripts >}}

## json:open

Open a file and read its content as JSON, then return it.

**Parameters**
- `filename`: path to JSON file

**Return value** `UserType`: json value

**Author**
- [@Gryfenfer97](https://github.com/Gryfenfer97)

**Example**
{{< highlight_arkscript >}}
(let json (json:open "test.json"))
{{< /highlight_arkscript >}}

## json:fromString

Takes a String as an argument, representing a JSON object, and return it parsed as a jsonObject.

**Parameters**
- `jsonObject`: JSON stored in a string

**Return value** `UserType`: json value

**Author**
- [@Gryfenfer97](https://github.com/Gryfenfer97)

**Example**
{{< highlight_arkscript >}}
(let json (json:fromString "{\"hello\": \"world\", \"key\": 12}"))
{{< /highlight_arkscript >}}

## json:get

Retrieve a value from a jsonObject by its key.

**Parameters**
- `jsonObject`: JSON object
- `key`: a string if the object is a dictionnary, a number if it's an array

**Return value** the stored value

**Author**
- [@Gryfenfer97](https://github.com/Gryfenfer97)

**Example**
{{< highlight_arkscript >}}
(let json (json:fromString "{\"hello\": \"world\", \"key\": 12}"))
(print (json:get json "hello"))  # world
{{< /highlight_arkscript >}}

## json:toString

Takes a jsonObject and transforms it into a String.

**Parameters**
- `jsonObject`: JSON object

**Return value** `String`: the string representation of the object

**Author**
- [@Gryfenfer97](https://github.com/Gryfenfer97)

**Example**
{{< highlight_arkscript >}}
(let json (json:fromString "{\"hello\": \"world\", \"key\": 12}"))
(print (json:toString json))  # {"hello": "world", "key": 12}
{{< /highlight_arkscript >}}

## json:set

Modify a value in a jsonObject, given a key (String) and a new value.

**Parameters**
- `jsonObject`: JSON object
- `key`: a string
- `value`: any type

**Return value** `nil`

**Author**
- [@Gryfenfer97](https://github.com/Gryfenfer97)

**Example**
{{< highlight_arkscript >}}
(let json (json:fromString "{\"hello\": \"world\", \"key\": 12}"))
(json:set json "hello" 14)
(print json)  # {"hello": 14, "key": 12}
{{< /highlight_arkscript >}}

## json:fromList

Take a list of an even number of values, the even ones are the keys (String) and the odd ones the values, and build a jsonObject from it.

**Parameters**
- `jsonAsList`: JSON stored in an ArkScript list

**Return value** `UserType`: json value

**Author**
- [@Gryfenfer97](https://github.com/Gryfenfer97)

**Example**
{{< highlight_arkscript >}}
(let json (json:fromList [
  "key" "value"
  "keybis" 12
  "this_is_an_array" [1 2 3]
  "bool" true
  "subobject" (json:fromList [
      "a" 1
      "b" 2 ])]))
{{< /highlight_arkscript >}}

## json:write

Take a jsonObject and a filename (String), and write the jsonObject to the file. The file will be created if it doesn't exist, otherwise all the previous content will be wiped before writting to it.

**Parameters**
- `jsonObject`: JSON object
- `filename`: a string

**Return value** `nil`

**Author**
- [@Gryfenfer97](https://github.com/Gryfenfer97)

**Example**
{{< highlight_arkscript >}}
(let json (json:fromList [
  "key" "value"
  "keybis" 12
  "this_is_an_array" [1 2 3]
  "bool" true
  "subobject" (json:fromList [
    "a" 1
    "b" 2 ])]))

(json:write json "filename.json")
{{< /highlight_arkscript >}}

## json:len

**Parameters**
- `jsonObject`: JSON object

**Return value** `Number`: size of the given JSON object

**Author**
- [@Gryfenfer97](https://github.com/Gryfenfer97)

**Example**
{{< highlight_arkscript >}}
(let json (json:fromList [
  "key" "value"
  "keybis" 12
  "this_is_an_array" [1 2 3]
  "bool" true
  "subobject" (json:fromList [
    "a" 1
    "b" 2 ])]))
(json:len json)  # 5
{{< /highlight_arkscript >}}
