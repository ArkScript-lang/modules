# http Server

## Functions

Important note related to the routes: in case the given content argument doesn't return the wanted values, it will silently fail to avoid crashing the server.

### http:server:create

Create the server (only one per VM may be instanciated).

Example:

```clojure
(http:server:create)
```

### Route handlers

- http:server:get
- http:server:post
- http:server:put
- http:server:delete

Create a route to answer the specified request type.

The route must be a String, the content must be a Function.

The function must return a list: `[status-code content type]`, with `type` being the mimetype of `content`.

If no params were sent, then params will be `nil`. The same applies to the matches.

In the case of a GET request, the body will always be `nil`.

Example:

```clojure
(http:server:create)
(http:server:get "/hi" "this is my fabulous content" "text/plain")

# both matches and params can be nil
(http:server:get "/numbers/(\\d+)" (fun (matches body params) {
    (print (len matches))  # 1
    (let number (toNumber (@ matches 0)))
    (print number)  # the matched number

    # return the given number + 12 as a string
    # (returned content must be a string)
    [200 (toString (+ 12 number)) "text/plain"]
}))
```

### http:server:stop

Stop a server.

Returns `nil`.

Example:

```clojure
(http:server:stop)
```

### http:server:listen

Setup the server to listen forever on a given host (String) and port (Number). Should only be called once after having setup all the routes.

The port is optional if the host is `"0.0.0.0"` (aka *bind to all interfaces*).

Returns `nil`.

Example:

```clojure
(http:server:create)

(http:server:get "/hi" "this is my fabulous content")
# more routes...

(http:server:listen "localhost" 1234)
```

### http:server:setMountPoint

Mount a given directory to a specific location.

You can mount a directory to multiple locations, thus creating a search order.

Returns a Boolean: true if it worked, false if the base directory doesn't exist.

Example:

```clojure
(http:server:create)

# mount / to ./www
(http:server:setMountPoint "/" "./www")

# mount /public to ./www1 and ./www2 directories
(http:server:setMountPoint "/public" "/www1")  # 1st order to search
(http:server:setMountPoint "/public" "/www2")  # 2nd order to search
```

### http:server:rmMountPoint

Remove a mount point. Returns false if the mount point can't be found, true otherwise.

Example:

```clojure
(http:server:create)

# mount / to ./www
(http:server:setMountPoint "/" "./www")

# mount /public to ./www1 and ./www2 directories
(http:server:setMountPoint "/public" "/www1")  # 1st order to search
(http:server:setMountPoint "/public" "/www2")  # 2nd order to search

# remove mount /
(http:server:rmMountPoint "/")
```

### http:server:setFileExtAndMimetypeMapping

Map a file extension to a mimetype.

Returns `nil`.

Built-in mappings:

Extension | MIME Type
--------- | ---------
txt | text/plain
html, htm | text/html
css | text/css
jpeg, jpg | image/jpg
png | image/png
gif | image/gif
svg | image/svg+xml
ico | image/x-icon
json | application/json
pdf | application/pdf
js | application/javascript
wasm | application/wasm
xml | application/xml
xhtml | application/xhtml+xml

Example:

```clojure
(http:server:setFileExtAndMimetypeMapping "cc" "text/x-c")
```

### http:server:enableLogger

Set the logging level, by default 0.

Returns `nil`.

Logger level 1 will display the timestamp, request method, path and response status.  
Level 2 will add the request body as well.

Example:

```clojure
# set logger level to 1
(http:server:enableLogger)

# select a given logger level, here 3
(http:server:enableLogger 3)
```