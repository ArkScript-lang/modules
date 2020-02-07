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