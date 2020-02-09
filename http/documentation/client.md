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

Returns a list: `[status, body]`.

Example:

```clojure
{
    (let headers (httpCreateHeaders
        "Accept-Encoding" "gzip, deflate"
    ))
    (let cli (httpCreateClient "localhost" 1234))

    (mut output (httpClientGet cli "/hi" headers))
    (print (@ output 0))  # print the status
}
```