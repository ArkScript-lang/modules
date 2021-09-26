#include <http_module.hpp>

// module functions mapping
ARK_API mapping* getFunctionsMapping()
{
    mapping* map = mapping_create(29);

    // Server
    mapping_add(map[0], "http:server:create", http_create_server);
    mapping_add(map[1], "http:server:get", http_server_get);
    mapping_add(map[2], "http:server:post", http_server_post);
    mapping_add(map[3], "http:server:put", http_server_put);
    mapping_add(map[4], "http:server:delete", http_server_delete);
    mapping_add(map[5], "http:server:stop", http_server_stop);
    mapping_add(map[6], "http:server:listen", http_server_listen);
    mapping_add(map[7], "http:server:setMountPoint", http_server_set_mount_point);
    mapping_add(map[8], "http:server:rmMountPoint", http_server_remove_mount_point);
    mapping_add(map[9], "http:server:setFileExtAndMimetypeMapping", http_server_set_fext_mimetype);
    mapping_add(map[10], "http:server:enableLogger", http_server_enable_logger);

    // Client
    mapping_add(map[11], "http:headers:create", http_create_headers);
    mapping_add(map[12], "http:client:create", http_create_client);
    mapping_add(map[13], "http:client:get", http_client_get);
    mapping_add(map[14], "http:params:create", http_create_params);
    mapping_add(map[15], "http:params:toList", http_params_tolist);
    mapping_add(map[16], "http:client:post", http_client_post);
    mapping_add(map[17], "http:client:put", http_client_put);
    mapping_add(map[18], "http:client:delete", http_client_delete);
    mapping_add(map[19], "http:client:setFollowLocation", http_client_set_follow_location);

    // Client
    mapping_add(map[20], "http:client:setConnectionTimeout", http_client_set_co_timeout);
    mapping_add(map[21], "http:client:setReadTimeout", http_client_set_read_timeout);
    mapping_add(map[22], "http:client:setWriteTimeout", http_client_set_write_timeout);
    mapping_add(map[23], "http:client:setBasicAuth", http_client_set_basic_auth);
    mapping_add(map[24], "http:client:setBearerTokenAuth", http_client_set_bearer_token_auth);
    mapping_add(map[25], "http:client:setKeepAlive", http_client_set_keep_alive);
    mapping_add(map[26], "http:client:setProxy", http_client_set_proxy);
    mapping_add(map[27], "http:client:setProxyBasicAuth", http_client_set_proxy_basic_auth);
    mapping_add(map[28], "http:client:setProxyBearerTokenAuth", http_client_set_proxy_bearer_token_auth);

    return map;
}