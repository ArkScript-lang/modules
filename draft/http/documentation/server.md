@page http_server http Server

Important note related to the routes: in case the given content argument doesn't return the wanted values, it will silently fail to avoid crashing the server.

## http:server:create

Create the server (only one per VM may be instanciated).

**Parameters**
- None.

**Return value** `nil`

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
~~~~{.lisp}
(import "http.arkm")
(http:server:create)
~~~~

## Route handlers

- `http:server:get`
- `http:server:post`
- `http:server:put`
- `http:server:delete`

Create a route to answer the specified request type.

**Parameters**
- `route`: a string representing the route to handle. The route can receive matches with the regex syntax, eg `/(\\d+)` to match on numbers
- `handler`: a function returning a list `[status-code content type]`
    - `status-code`: number, representing an HTTP code
    - `content`: string
    - `type`: string, the mimetype for `content`

The typical `handler` function looks like this: `(fun (matches body params) ...)`:
- `matches` is a list of matches (as strings). If none were found, it will be `nil`
- `body` is a string, the request's body ; for GET requests this is always `nil`
- `params` is a `UserType<http:Params>` sent to the function if any, otherwise `nil`

**Return value** `nil`

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
~~~~{.lisp}
(import "http.arkm")

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
~~~~

## http:server:stop

Stop a server.

**Parameters**
- None.

**Return value** `nil`

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
~~~~{.lisp}
(import "http.arkm")

(http:server:create)
...
(http:server:stop)  # this can be used in routes

(http:server:get "/shutdown" (fun (matches body params) {
    (http:server:stop)
    # this will never be sent to the user be cause the server shut down before
    [200 "Going down" "text/plain"]
}))
~~~~

## http:server:listen

Setup the server to listen forever. Should only be called once after having setup all the routes.

**Parameters**
- `host`: a string ; an IP address or `localhost`
- `port`: a number, optional if the host is `0.0.0.0` (= *bind to all interfaces*)

**Return value** `nil`

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
~~~~{.lisp}
(import "http.arkm")

(http:server:create)

(http:server:get "/hi" "this is my fabulous content")
# more routes

(http:server:listen "localhost" 1234)
~~~~

## http:server:setMountPoint

Mount a given directory to a specific location. You can mount a directory to multiple locations, thus creating a search order.

**Parameters**
- `mountpoint`: string, a route
- `port`: string, a folder on the computer

**Return value** `bool`, true if it worked, false if the base directory doesn't exist.

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
~~~~{.lisp}
(import "http.arkm")

(http:server:create)

# mount / to ./www
(http:server:setMountPoint "/" "./www")

# mount /public to ./www1 and ./www2 directories
(http:server:setMountPoint "/public" "/www1")  # 1st order to search
(http:server:setMountPoint "/public" "/www2")  # 2nd order to search
~~~~

## http:server:rmMountPoint

Remove a mount point.

**Parameters**
- `mountpoint`: string, a mountpoint name

**Return value** `bool`, false if the mount point can't be found, true otherwise.

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
~~~~{.lisp}
(import "http.arkm")

(http:server:create)

# mount / to ./www
(http:server:setMountPoint "/" "./www")

# mount /public to ./www1 and ./www2 directories
(http:server:setMountPoint "/public" "/www1")  # 1st order to search
(http:server:setMountPoint "/public" "/www2")  # 2nd order to search

# remove mount /
(http:server:rmMountPoint "/")
~~~~

## http:server:setFileExtAndMimetypeMapping

Map a file extension to a mimetype.

**Return value** `nil`

**Parameters**
- `extension`: string, a file extension
- `mimetype`: string, a mimetype

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

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
~~~~{.lisp}
(http:server:setFileExtAndMimetypeMapping "cc" "text/x-c")
~~~~

## http:server:enableLogger

Set the logging level, by default 0.

**Parameters**
- `loglevel`: number

Logger level 1 will display the timestamp, request method, path and response status.  
Level 2 will add the request body as well.

**Return value** `nil`

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
~~~~{.lisp}
(import "http.arkm")

(http:server:create)

# set logger level to 1
(http:server:enableLogger)

# select a given logger level, here 3
(http:server:enableLogger 3)
~~~~
