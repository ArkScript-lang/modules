@page http_module HTTP module

A module to play with HTTP requests, using [cpp-httplib](https://github.com/yhirose/cpp-httplib) (MIT License).

Disclaimer: this module does not handle
* multipart/form-data POST requests
* content received with content receiver
* content sent with content receiver
* chunked transfer encoding
* digest authentication (needs OpenSSL and libcrypto for that)

## http:headers

Create a header map to use with the http client.

**Parameters**
They work as pairs with:
- `name`: string
- `value`: string

**Return value** `UserType<httpHeaders>`

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
~~~~{.lisp}
(let headers (http:headers
    # pair 0
    "Accept-Encoding" "gzip, deflate"
    # pair 1
    "Content-Type" "application/json"))
~~~~

## http:client

Create a http client to query a server.

**Parameters**
- `host`: string, you must not put the protocol `http://` nor `https://`
- `port`: number

**Return value** `List` if the request succeeded: `[status, body]`, otherwise `nil`

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
~~~~{.lisp}
(let cli (http:client "localhost" 1234))
~~~~

## http:get

Get content from an online resource.

**Parameters**
- `client`: `UserType<httpClient>`
- `route`: string
- `headers`: `UserType<httpHeaders>`, optional, always come last

**Return value** `List` if the request succeeded: `[status, body]`, otherwise `nil`

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
~~~~{.lisp}
(let headers (http:headers:create
    "Accept-Encoding" "gzip, deflate"))
(let cli (http:client "localhost" 1234))

(mut output (http:get cli "/hi" headers))
(if (nil? output)
    (print "couldn't reach the server")
    (print (@ output 0)))  # print the status
~~~~

## http:post, http:put, http:patch, http:delete

Make a POST / PUT / PATCH / DELETE request with a String as the request's body.

**Parameters**
- `client`: `UserType<httpClient>`
- `route`: string
- `body`: string (request body)
- `mimetype`: string, optional (defaults to `text/plain`)
- `headers`: `UserType<httpHeaders>`, optional, always come last

**Return value** `List` if the request succeeded: `[status, body]`, otherwise `nil`

**Author**
- [@SuperFola](https://github.com/SuperFola)

**Example**
~~~~{.lisp}
(let cli (http:client "localhost" 1234))

(mut output (http:post cli "/hi" "{\"key\": 5}" "application/json" headers))
(if (nil? output)
    (print "couldn't reach the server")
    (print (@ output 0)))  # print the status
~~~~
