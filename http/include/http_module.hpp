#ifndef http_module
#define http_module

#include <Ark/Module.hpp>

Value http_create_server(std::vector<Value>& n);
Value http_server_get(std::vector<Value>& n);
Value http_server_stop(std::vector<Value>& n);
Value http_server_listen(std::vector<Value>& n);
Value http_server_bind_to_any_port(std::vector<Value>& n);
Value http_server_listen_after_bind(std::vector<Value>& n);
Value http_server_set_mount_point(std::vector<Value>& n);
Value http_server_remove_mount_point(std::vector<Value>& n);
Value http_server_set_fext_mimetype(std::vector<Value>& n);
Value http_server_enable_logger(std::vector<Value>& n);

Value http_create_headers(std::vector<Value>& n);
Value http_create_client(std::vector<Value>& n);
Value http_client_get(std::vector<Value>& n);
Value http_create_params(std::vector<Value>& n);
Value http_client_post(std::vector<Value>& n);
Value http_client_put(std::vector<Value>& n);
Value http_client_delete(std::vector<Value>& n);
Value http_client_set_follow_location(std::vector<Value>& n);

#endif