#include <http_module.hpp>

// module functions mapping
ARK_API mapping* getFunctionsMapping()
{
    static mapping map[] = {
        { "http:headers", http_create_headers },
        { "http:client", http_create_client },
        { "http:get", http_client_get },
        { "http:post", http_client_post },
        { "http:put", http_client_put },
        { "http:delete", http_client_delete },
        { "http:patch", http_client_patch },
        { "http:setFollowLocation", http_client_set_follow_location },
        { "http:setConnectionTimeout", http_client_set_co_timeout },
        { "http:setReadTimeout", http_client_set_read_timeout },
        { "http:setWriteTimeout", http_client_set_write_timeout },
        { "http:setBasicAuth", http_client_set_basic_auth },
        { "http:setBearerTokenAuth", http_client_set_bearer_token_auth },
        { "http:setKeepAlive", http_client_set_keep_alive },
        { "http:setProxy", http_client_set_proxy },
        { "http:setProxyBasicAuth", http_client_set_proxy_basic_auth },
        { "http:setProxyBearerTokenAuth", http_client_set_proxy_bearer_token_auth },

        { nullptr, nullptr }
    };

    return map;
}