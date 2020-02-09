# http Client

## Functions

### httpCreateHeaders

Create a header map to use with the http client.

You can give it the headers you want when creating it.

Example:

```clojure
(let headers (httpCreateHeaders
    "Accept-Encoding" "gzip, deflate"
))
```

### httpCreateClient

Create an http client to query a server.

Must give an host (String) and a port number.

Example:

```clojure
(let cli (httpCreateClient "localhost" 1234))
```

### httpClientGet

Get content from an online ressource.

Must give a client (httpClient) and a route, headers are optionals.

Returns a list if the request succeeded: `[status, body]`, otherwise `nil`.

Example:

```clojure
{
    (let headers (httpCreateHeaders
        "Accept-Encoding" "gzip, deflate"
    ))
    (let cli (httpCreateClient "localhost" 1234))

    (mut output (httpClientGet cli "/hi" headers))
    (if (nil? output)
        (print "couldn't reach the server")
        (print (@ output 0))  # print the status
    )
}
```

### httpCreateParams

Used to create a parameter list for a POST request (`application/x-www-form-urlencoded`).

It works like `httpCreateHeaders`, you need to give an even number of Strings (key -> value mapping).

Example:

```clojure
(let params (httpCreateParams
    "name" "john"
    "note" "coder"
))
```

### httpClientPost

Make a POST request with either a String (request type would be `text/plain`) as the content or `httpParams` (request type would be `application/x-www-form-urlencoded`).

Returns a list if the request succeeded: `[status, body]`, otherwise `nil`.

Example:

```clojure
{
    (let cli (httpCreateClient "localhost" 1234))

    (mut output (httpClientPost cli "/form" "hello world!"))
    (if (nil? output)
        (print "couldn't reach the server")
        (print (@ output 0))  # prints status of the request
    )

    (let params (httpCreateParams
        "name" "john"
        "note" "coder"
    ))
    (set output (httpClientPost "/form-bis" params))
    (if (nil? output)
        (print "couldn't reach the server")
        (print (@ output 0))  # prints status of the request
    )
}
```

### httpClientPut

Make a PUT request with either a String (request type would be `text/plain`) as the content or `httpParams` (request type would be `application/x-www-form-urlencoded`).

Returns a list if the request succeeded: `[status, body]`, otherwise `nil`.

Works exactly like `httpClientPost`.

### httpClientDelete

Make a DELETE request with an optional data field (request type would be `text/plain`).

Returns a list if the request succeeded: `[status, body]`, otherwise `nil`.

Example:

```clojure
{
    (let cli (httpCreateClient "localhost" 1234))

    (mut output (httpClientDelete cli "/delete-me"))
    (if (nil? output)
        (print "couldn't reach the server")
        (print (@ output 0))  # prints status of the request
    )
}
```

### httpClientSetFollowLocation

Choose if the request should follow the redirection or not.

Returns `nil`.

Example:

```clojure
{
    (let cli (httpCreateClient "yahoo.com" 80))

    (mut output (httpClientGet cli "/"))
    (print (@ output 0))  # status: 301

    (httpClientSetFollowLocation cli true)
    (set output (httpClientGet cli "/"))
    (print (@ output 0))  # status: 200
}
```