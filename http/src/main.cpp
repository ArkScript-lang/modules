#include <http_module.hpp>

// module functions mapping
ARK_API_EXPORT Mapping_t getFunctionsMapping()
{
    Mapping_t map;

    // Server
    map["http:server:create"] = http_create_server;
    map["http:server:get"] = http_server_get;
    map["http:server:post"] = http_server_post;
    map["http:server:put"] = http_server_put;
    map["http:server:delete"] = http_server_delete;
    map["http:server:stop"] = http_server_stop;
    map["http:server:listen"] = http_server_listen;
    map["http:server:setMountPoint"] = http_server_set_mount_point;
    map["http:server:rmMountPoint"] = http_server_remove_mount_point;
    map["http:server:setFileExtAndMimetypeMapping"] = http_server_set_fext_mimetype;
    map["http:server:enableLogger"] = http_server_enable_logger;

    // Client
    map["http:headers:create"] = http_create_headers;
    map["http:client:create"] = http_create_client;
    map["http:client:get"] = http_client_get;
    map["http:params:create"] = http_create_params;
    map["http:client:post"] = http_client_post;
    map["http:client:put"] = http_client_put;
    map["http:client:delete"] = http_client_delete;
    map["http:client:setFollowLocation"] = http_client_set_follow_location;

    map["http:client:setConnectionTimeout"] = http_client_set_co_timeout;
    map["http:client:setReadTimeout"] = http_client_set_read_timeout;
    map["http:client:setWriteTimeout"] = http_client_set_write_timeout;
    map["http:client:setBasicAuth"] = http_client_set_basic_auth;
    map["http:client:setBearerTokenAuth"] = http_client_set_bearer_token_auth;
    map["http:client:setKeepAlive"] = http_client_set_keep_alive;
    map["http:client:setProxy"] = http_client_set_proxy;
    map["http:client:setProxyBasicAuth"] = http_client_set_proxy_basic_auth;
    map["http:client:setProxyBearerTokenAuth"] = http_client_set_proxy_bearer_token_auth;

    return map;
}