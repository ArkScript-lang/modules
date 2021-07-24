# http Server

## Functions

Important note related to the routes: in case the given content argument doesn't return the wanted values, it will silently fail to avoid crashing the server.

### http:server:create

Create a server.

Example:

```clojure
(let srv (http:server:create))
```

### http:server:get

Create a route to answer GET requests.

The route must be a String, the content can be either a String or a Function. If the content is a String, the mimetype will be set to `text/plain` or to the value of a fourth optional argument which must be a String.

If the route accepts matches (eg `"/numbers/(\\d+)"`), the content must be a Function accepting arguments: matches and params.  
If the route doesn't accept matches, it takes a single argument: params.  
The function must return a list: `[status-code content type]`, with `type` being the mimetype of `content`.

If no params were sent, then params will be `nil`.

Example:

```clojure
(let srv (http:server:create))
(http:server:get "/hi" "this is my fabulous content" "text/plain")

(http:server:get "/numbers/(\\d+)" (fun (matches params) {
    (print (len matches))  # 1
    (let number (toNumber (@ matches 0)))
    (print number)  # the matched number

    # return the given number + 12 as a string
    # (returned content must be a string)
    [200 (toString (+ 12 number)) "text/plain"]
}))
```

### http:server:post

Create a route to answer POST requests.

The route must be a String, the content must be Function.

If the route accepts matches (eg `"/numbers/(\\d+)"`), the content must be a Function accepting arguments: matches, body and params.  
If the route doesn't accept matches, it takes only: body and params.  
The function must return a list: `[status-code content type]`, with `type` being the mimetype of `content`.

If no params were sent, then params will be `nil`.

### http:server:put

Works the same way as `http:server:post`.

### http:server:delete

Works the same way as `http:server:post`.

### http:server:stop

Stop a server.

Returns `nil`.

Example:

```clojure
(http:server:stop)
```

### http:server:listen

Setup the server to listen forever on a given host (String) and port (Number). Should be called after having setup all the routes.

The port is optional if the host is `"0.0.0.0"` (aka *bind to all interfaces*).

Returns `nil`.

Example:

```clojure
(let srv (http:server:create))

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
(let srv (http:server:create))

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
(let srv (http:server:create))

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
txt       | text/plain
html, htm | text/html
css       | text/css
jpeg, jpg | image/jpg
png       | image/png
gif       | image/gif
svg       | image/svg+xml
ico       | image/x-icon
json      | application/json
pdf       | application/pdf
js        | application/javascript
wasm      | application/wasm
xml       | application/xml
xhtml     | application/xhtml+xml

Example:

```clojure
(http:server:setFileExtAndMimetypeMapping "cc" "text/x-c")
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