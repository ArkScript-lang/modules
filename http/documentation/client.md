# http Client

## Functions

### http:headers:create

Create a header map to use with the http client.

You can give it the headers you want when creating it.

Example:

```clojure
(let headers (http:headers:create
    "Accept-Encoding" "gzip, deflate"
))
```

### http:client:create

Create an http client to query a server.

Must give an host (String) and a port number.

Example:

```clojure
(let cli (http:client:create "localhost" 1234))
```

### http:client:get

Get content from an online ressource.

Must give a client (httpClient) and a route, headers are optionals.

Returns a list if the request succeeded: `[status, body]`, otherwise `nil`.

Example:

```clojure
(let headers (http:headers:create
    "Accept-Encoding" "gzip, deflate"
))
(let cli (http:client:create "localhost" 1234))

(mut output (http:client:get cli "/hi" headers))
(if (nil? output)
    (print "couldn't reach the server")
    (print (@ output 0)))  # print the status
```

### http:client:post

Make a POST request with either a String as the request's body or `httpParams` (request type would be `application/x-www-form-urlencoded`).

If the request's body is a String, you can pass an optional String argument `content-type` (defaults to `text/plain` when not given).

A fifth (or fourth, depending on the form of the request) optional argument can be added: `headers`. They always come last.

Returns a list if the request succeeded: `[status, body]`, otherwise `nil`.

Example:

```clojure
(let cli (http:client:create "localhost" 1234))

# the "text/plain" argument is optional here
(mut output (http:client:post cli "/form" "hello world!" "text/plain"))
(if (nil? output)
    (print "couldn't reach the server")
    (print (@ output 0)))  # prints status of the request

(let params (http:params:create
    "name" "john"
    "note" "coder"
))
# here, no need to add the content-type because we are sending parameters
(set output (http:client:post "/form-bis" params))
(if (nil? output)
    (print "couldn't reach the server")
    (print (@ output 0)))  # prints status of the request
```

### http:client:put

Make a PUT request with either a String as the request's body or `httpParams` (request type would be `application/x-www-form-urlencoded`).

If the request's body is a String, you can pass an optional String argument `content-type` (defaults to `text/plain` when not given).

A fifth (or fourth, depending on the form of the request) optional argument can be added: `headers`. They always come last.

Returns a list if the request succeeded: `[status, body]`, otherwise `nil`.

Works exactly like `http:client:post`.

### http:client:delete

Make a DELETE request with either a String as the request's body or `httpParams` (request type would be `application/x-www-form-urlencoded`).

If the request's body is a String, you can pass an optional String argument `content-type` (defaults to `text/plain` when not given).

A fifth (or fourth, depending on the form of the request) optional argument can be added: `headers`. They always come last.

Returns a list if the request succeeded: `[status, body]`, otherwise `nil`.

Works exactly like `http:client:post`.

### http:params:create

Used to create a parameter list for a POST/PUT/DELETE request (`application/x-www-form-urlencoded`).

It works like `http:headers:create`, you need to give an even number of Strings (key -> value mapping).

Example:

```clojure
(let params (http:params:create
    "name" "john"
    "note" "coder"
))
```

### http:client:setFollowLocation

Choose if the request should follow the redirection or not.

Returns `nil`.

Example:

```clojure
(let cli (http:client:create "yahoo.com" 80))

(mut output (http:client:get cli "/"))
(print (@ output 0))  # status: 301

(http:client:setFollowLocation cli true)
(set output (http:client:get cli "/"))
(print (@ output 0))  # status: 200
```