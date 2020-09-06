# JSON module

## Functions

### json:open

Open a file and read its content as JSON, then return it.

Example:

```clojure
(let json (json:open "test.json"))
```

### json:fromString

Takes a String as an argument, representing a JSON object, and return it parsed as a jsonObject.

Example:
```clojure
(let json (json:fromString "{\"hello\": \"world\", \"key\": 12}"))
```

### json:get

Retrieve a value from a jsonObject by it's key (a String).

Example:

```clojure
(let json (json:fromString "{\"hello\": \"world\", \"key\": 12}"))
(print (json:get json "hello"))  # world
```

### json:toString

Takes a jsonObject and transforms it into a String.

Example:

```clojure
(let json (json:fromString "{\"hello\": \"world\", \"key\": 12}"))
(print (json:toString json))  # {"hello": "world", "key": 12}
```

### json:set

Modify a value in a jsonObject, given a key (String) and a new value.

Return `nil`.

Example:

```clojure
(let json (json:fromString "{\"hello\": \"world\", \"key\": 12}"))
(json:set json "hello" 14)
(print json)  # {"hello": 14, "key": 12}
```

### json:fromList

Take a list of an even number of values, the even ones are the keys (String) and the odd ones the values, and build a jsonObject from it.

Example:

```clojure
(let json (json:fromList [
    "key" "value"
    "keybis" 12
    "this_is_an_array" [1 2 3]
    "bool" true
    "subobject" (json:fromList [
        "a" 1
        "b" 2
    ])
]))
```

### json:write

Take a jsonObject and a filename (String), and write the jsonObject to the file. The file will be created if it doesn't exist, otherwise all the previous content will be wiped before writting to it.

Example:

```clojure
(let json (json:fromList [
    "key" "value"
    "keybis" 12
    "this_is_an_array" [1 2 3]
    "bool" true
    "subobject" (json:fromList [
        "a" 1
        "b" 2
    ])
]))

(json:write json "filename.json")
```