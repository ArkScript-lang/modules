#include <http_module.hpp>

// module functions mapping
ARK_API mapping* getFunctionsMapping()
{
    static mapping map[] = {
        // Server
        { "http:server:create", http_create_server },
        { "http:server:get", http_server_get },
        { "http:server:post", http_server_post },
        { "http:server:put", http_server_put },
        { "http:server:delete", http_server_delete },
        { "http:server:stop", http_server_stop },
        { "http:server:listen", http_server_listen },
        { "http:server:setMountPoint", http_server_set_mount_point },
        { "http:server:rmMountPoint", http_server_remove_mount_point },
        { "http:server:setFileExtAndMimetypeMapping", http_server_set_fext_mimetype },
        { "http:server:enableLogger", http_server_enable_logger },

        // Client
        { "http:headers:create", http_create_headers },
        { "http:client:create", http_create_client },
        { "http:client:get", http_client_get },
        { "http:params:create", http_create_params },
        { "http:params:toList", http_params_tolist },
        { "http:client:post", http_client_post },
        { "http:client:put", http_client_put },
        { "http:client:delete", http_client_delete },
        { "http:client:setFollowLocation", http_client_set_follow_location },

        // Client
        { "http:client:setConnectionTimeout", http_client_set_co_timeout },
        { "http:client:setReadTimeout", http_client_set_read_timeout },
        { "http:client:setWriteTimeout", http_client_set_write_timeout },
        { "http:client:setBasicAuth", http_client_set_basic_auth },
        { "http:client:setBearerTokenAuth", http_client_set_bearer_token_auth },
        { "http:client:setKeepAlive", http_client_set_keep_alive },
        { "http:client:setProxy", http_client_set_proxy },
        { "http:client:setProxyBasicAuth", http_client_set_proxy_basic_auth },
        { "http:client:setProxyBearerTokenAuth", http_client_set_proxy_bearer_token_auth },

        { nullptr, nullptr }
    };

    return map;
}