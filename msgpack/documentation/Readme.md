# msgpack module documentation

A module for **msgpack**, an efficient binary serialization format like JSON, but smaller and faster.

## msgpack

+ **(msgpack:sbuffer)** : construct the buffer that must contain packed data
+ **(msgpack:objectHandle)** : construct a msgpack::object_handle. It must manage and return msgpack::object
+ **(msgpack:object msgpack_object_handle)** : construct the msgpack::object. It must contain the unpacked object
+ **(msgpack:pack sbuffer ark_object)** : data packing
+ **(msgpack:unpack sbuffer msgpack_object_handle)** : for data unpacking
+ **(msgpack:convert msgpack_object)** : convert msgpack::object to ark usable object

### Example

``` clojure
(import "msgpack.arkm")

# buffer
(let sbuf (msgpack:sbuffer))

# packing
(msgpack:pack sbuf (list 1 2.45 "Yellow Bow !" true nil false (list 1 2.45 "hello !")))

# unpacking
# msgpack object handle
(mut oh (msgpack:objectHandle))

(msgpack:unpack sbuf oh)

# msgpack object
(let o (msgpack:object oh))

# convert msgpackObj to ark object
(mut dst (msgpack:convert o))
(print "dst : " dst)
```
