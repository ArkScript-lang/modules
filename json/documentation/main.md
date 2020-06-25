# json documentation module

We have an implementation of a json object. The possibility to print, read and write json in Arkscript with some functions : 
- **open** : A function that take the filename and return the json object
- **fromString** : A function that take a String and return the corresponding json object.
- **get** : A function that take a json object and a String (the key) and return the value corresponding to the key
- **toString** : A function that take a json object and return the serialized String
- **jset** : A function that take a json Object, a key and a value. returning nothing.
- **fromList** : A function that convert a list into a json object.
- **write** : A function that take a json object and a filename. The function write the json object in a file.

## Exemple 

```clojure
{
    (import "/home/jamil/Projects/Ark/lib/json.arkm")

	(let json_object (open "test.json"))

	(print json_object)

	(let other_json_object (fromList [
		"key" "value"
    	"test" true
		"nested" ["sd" json_object]
		"nesd" (fromList ["a" "b"])
	]))

	(print (toString other_json_object))

	(jset json_object "test" 123)

	(write json_object "other_file.json")
}
```