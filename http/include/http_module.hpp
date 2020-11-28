#ifndef http_module
#define http_module

// must be included first, before any #include windows.h
#include <httplib.hpp>

#include <Ark/Module.hpp>

#include <vector>
#include <memory>
#include <list>

using namespace httplib;

// utils
std::list<Params>& get_params();
std::list<Headers>& get_headers();
std::list<Client>& get_clients();
int& get_logger_level();
bool& get_error_handler();
Server& create_server();
UserType::ControlFuncs* get_cfs_client();
UserType::ControlFuncs* get_cfs_header();
UserType::ControlFuncs* get_cfs_param();
UserType::ControlFuncs* get_cfs_server();

// server
Value http_create_server(std::vector<Value>& n, Ark::VM* vm);
Value http_server_get(std::vector<Value>& n, Ark::VM* vm);
Value http_server_post(std::vector<Value>& n, Ark::VM* vm);
Value http_server_put(std::vector<Value>& n, Ark::VM* vm);
Value http_server_delete(std::vector<Value>& n, Ark::VM* vm);
Value http_server_stop(std::vector<Value>& n, Ark::VM* vm);
Value http_server_listen(std::vector<Value>& n, Ark::VM* vm);
Value http_server_set_mount_point(std::vector<Value>& n, Ark::VM* vm);
Value http_server_remove_mount_point(std::vector<Value>& n, Ark::VM* vm);
Value http_server_set_fext_mimetype(std::vector<Value>& n, Ark::VM* vm);
Value http_server_enable_logger(std::vector<Value>& n, Ark::VM* vm);

// client
Value http_create_headers(std::vector<Value>& n, Ark::VM* vm);
Value http_create_client(std::vector<Value>& n, Ark::VM* vm);
Value http_client_get(std::vector<Value>& n, Ark::VM* vm);
Value http_create_params(std::vector<Value>& n, Ark::VM* vm);
Value http_client_post(std::vector<Value>& n, Ark::VM* vm);
Value http_client_put(std::vector<Value>& n, Ark::VM* vm);
Value http_client_delete(std::vector<Value>& n, Ark::VM* vm);
Value http_client_set_follow_location(std::vector<Value>& n, Ark::VM* vm);

Value http_client_set_co_timeout(std::vector<Value>& n, Ark::VM* vm);
Value http_client_set_read_timeout(std::vector<Value>& n, Ark::VM* vm);
Value http_client_set_write_timeout(std::vector<Value>& n, Ark::VM* vm);
Value http_client_set_basic_auth(std::vector<Value>& n, Ark::VM* vm);
Value http_client_set_bearer_token_auth(std::vector<Value>& n, Ark::VM* vm);
Value http_client_set_keep_alive(std::vector<Value>& n, Ark::VM* vm);
Value http_client_set_proxy(std::vector<Value>& n, Ark::VM* vm);
Value http_client_set_proxy_basic_auth(std::vector<Value>& n, Ark::VM* vm);
Value http_client_set_proxy_bearer_token_auth(std::vector<Value>& n, Ark::VM* vm);

#endif
