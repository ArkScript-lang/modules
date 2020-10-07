# Msgpack module documentation


Now, we propose two ways to use msgpack in Ark, the minimalist msgpack (with limited background object manipulation) and a more realistic msgpack module (with msgpack objects use in Ark) .

## **Msgpack**


+ **msgpackSBuffer()** : construct the buffer that must containes packed data ;
+ **msgpackObjHandle(sbuf)** : construct a msgpack::object_handle. It must manage and return msgpack::object ;
+ **msgpackObj(msgpack_object_handle)** : construct the msgpack::object. It must containe the unpacked object ;
+ **msgpackPack(sbuf object)** : data packing ;
+ **msgpackUnpack** : for data unpacking ;
+ **printO(msgpack_object)** : print msgpack::object ;
+ **msgpackConvert** : convert msgpack::object to ark usable object .



### Example

``` clojure
	(import "msgpack.bin")


	# sbuffer
	(let sbuf (msgpackSBuffer))

	# packing
	(msgpackPack sbuf (list 1 2.45 "Yellow Bow !" true nil false))

	# unpacking
	# msgpack object handle
	(let oh (msgpackUnpack sbuf))

	# msgpack object
	(let o (msgpackObj oh))
	(printO o)

	# convert msgpackObj to ark object
	(mut dst (msgpackConvert o))
	(print "dst : " dst)
```


### Comming soon


+ Sub list convert ;
+ Map pack, unpack and convert .