# HTTP module

A module to play with HTTP requests and create web servers, using [cpp-httplib](https://github.com/yhirose/cpp-httplib) (MIT License).

* [Server](server.md)
* [Client](client.md)

Disclaimer: this module does not handle
* multipart/form-data POST requests
* content received with content receiver
* content sent with content receiver
* chunked transfer encoding
* server-sent events
* multiple servers per program