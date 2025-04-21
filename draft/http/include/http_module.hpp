#ifndef http_module
#define http_module

// must be included first, before any #include windows.h
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

#include <Ark/Module.hpp>

#include <vector>
#include <string>
#include <memory>

using namespace httplib;
using namespace Ark;

// utils
std::vector<std::unique_ptr<Headers>>& get_headers();
std::vector<std::unique_ptr<Client>>& get_clients();
UserType::ControlFuncs* get_cfs_client();
UserType::ControlFuncs* get_cfs_header();

// client
Value http_create_headers(std::vector<Value>& n, VM* vm);
Value http_create_client(std::vector<Value>& n, VM* vm);
Value http_client_get(std::vector<Value>& n, VM* vm);
Value http_client_post(std::vector<Value>& n, VM* vm);
Value http_client_put(std::vector<Value>& n, VM* vm);
Value http_client_delete(std::vector<Value>& n, VM* vm);
Value http_client_patch(std::vector<Value>& n, VM* vm);
Value http_client_set_follow_location(std::vector<Value>& n, VM* vm);

Value http_client_set_co_timeout(std::vector<Value>& n, VM* vm);
Value http_client_set_read_timeout(std::vector<Value>& n, VM* vm);
Value http_client_set_write_timeout(std::vector<Value>& n, VM* vm);
Value http_client_set_basic_auth(std::vector<Value>& n, VM* vm);
Value http_client_set_bearer_token_auth(std::vector<Value>& n, VM* vm);
Value http_client_set_keep_alive(std::vector<Value>& n, VM* vm);
Value http_client_set_proxy(std::vector<Value>& n, VM* vm);
Value http_client_set_proxy_basic_auth(std::vector<Value>& n, VM* vm);
Value http_client_set_proxy_bearer_token_auth(std::vector<Value>& n, VM* vm);

#endif
