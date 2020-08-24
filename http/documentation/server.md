# http Server

## Functions

### http:server:create

Create a server.

Example:

```clojure
(let srv (http:server:create))
```

### http:server:get

Create a route to answer GET requests.

The route and the content should both be of type String.

Example:

```clojure
(let srv (http:server:create))
(http:server:get srv "/hi" "this is my fabulous content")
```

### http:server:stop

Stop a server.

Returns `nil`.

Example:

```clojure
(http:server:stop srv)
```

### http:server:listen

Setup the server to listen forever on a given host (String) and port (Number). Should be called after having setup all the routes.

The port is optional if the host is `"0.0.0.0"` (aka *bind to all interfaces*).

Returns `nil`.

Example:

```clojure
(let srv (http:server:create))

(http:server:get srv "/hi" "this is my fabulous content")
# more routes...

(http:server:listen srv "localhost" 1234)
```

### http:server:setMountPoint

Mount a given directory to a specific location.

You can mount a directory to multiple locations, thus creating a search order.

Returns a Boolean: true if it worked, false if the base directory doesn't exist.

Example:

```clojure
(let srv (http:server:create))

# mount / to ./www
(http:server:setMountPoint srv "/" "./www")

# mount /public to ./www1 and ./www2 directories
(http:server:setMountPoint srv "/public" "/www1")  # 1st order to search
(http:server:setMountPoint srv "/public" "/www2")  # 2nd order to search
```

### http:server:rmMountPoint

Remove a mount point. Returns false if the mount point can't be found, true otherwise.

Example:

```clojure
(let srv (http:server:create))

# mount / to ./www
(http:server:setMountPoint srv "/" "./www")

# mount /public to ./www1 and ./www2 directories
(http:server:setMountPoint srv "/public" "/www1")  # 1st order to search
(http:server:setMountPoint srv "/public" "/www2")  # 2nd order to search

# remove mount /
(http:server:rmMountPoint srv "/")
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
(http:server:setFileExtAndMimetypeMapping srv "cc" "text/x-c")
```

### http:server:enableLogger

Set the logging level, by default 0.

Returns `nil`.

TODO document the logging level

Example:

```clojure
{
    # set logger level to 1
    (http:server:enableLogger)

    # select a given logger level, here 3
    (http:server:enableLogger 3)
}
```