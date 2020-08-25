#ifndef http_module
#define http_module

#include <Ark/Module.hpp>

#include <httplib.hpp>
#include <vector>
#include <memory>

using namespace httplib;

// utils
std::vector<std::unique_ptr<Params>>& get_params();
std::vector<std::unique_ptr<Headers>>& get_headers();
std::vector<std::unique_ptr<Client>>& get_clients();
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

// params
Value http_create_headers(std::vector<Value>& n, Ark::VM* vm);
Value http_create_client(std::vector<Value>& n, Ark::VM* vm);
Value http_client_get(std::vector<Value>& n, Ark::VM* vm);
Value http_create_params(std::vector<Value>& n, Ark::VM* vm);
Value http_client_post(std::vector<Value>& n, Ark::VM* vm);
Value http_client_put(std::vector<Value>& n, Ark::VM* vm);
Value http_client_delete(std::vector<Value>& n, Ark::VM* vm);
Value http_client_set_follow_location(std::vector<Value>& n, Ark::VM* vm);

#endif