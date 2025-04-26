from typing import Any, Iterable
from wsgiref.simple_server import make_server
from wsgiref.types import StartResponse
from wsgiref.util import setup_testing_defaults


# A relatively simple WSGI application. It's going to print out the
# environment dictionary after being updated by setup_testing_defaults
def simple_app(environ: dict[str, Any], start_response: StartResponse) -> Iterable[bytes]:
    setup_testing_defaults(environ)

    status = '200 OK'
    headers = [('Content-type', 'text/plain; charset=utf-8')]

    start_response(status, headers)

    ret = [f"{key}: {environ[key]}\n".encode("utf-8")
           for key in (
               "SERVER_PORT", "CONTENT_LENGTH", "SERVER_PROTOCOL", "REQUEST_METHOD", "PATH_INFO", "QUERY_STRING",
               "CONTENT_TYPE", "HTTP_USER_AGENT", "HTTP_ACCEPT")]
    if environ["CONTENT_LENGTH"] and environ["CONTENT_LENGTH"] != "0":
        length = int(environ["CONTENT_LENGTH"])
        data = environ["wsgi.input"].read(length).decode("utf-8")
        ret.append(f"DATA: {data}\n".encode("utf-8"))
    else:
        ret.append("DATA: \n".encode("utf-8"))

    headers = [s for s in environ.keys() if
               s.startswith("HTTP_") and s not in ("HTTP_USER_AGENT", "HTTP_ACCEPT",
                                                   "HTTP_HOST", "HTTP_USER_AGENT",
                                                   "HTTP_CONNECTION")]
    content = ""
    for header in sorted(headers):
        name = header.replace("HTTP_", "")
        value = environ[header]
        content += f"{name}={value};"
    ret.append(f"HEADERS: {content}".encode("utf-8"))

    return ret


with make_server('', 8000, simple_app) as httpd:
    print("Serving on port 8000...")
    httpd.serve_forever()
