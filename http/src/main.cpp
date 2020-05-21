#include <http_module.hpp>

// module functions mapping
ARK_API_EXPORT Mapping_t getFunctionsMapping()
{
    Mapping_t map;

    map["httpCreateServer"] = http_create_server;
    map["httpServerGet"] = http_server_get;
    // TODO add httpServerPost, httpServerPut, httpServerDelete
    map["httpServerStop"] = http_server_stop;
    map["httpServerListen"] = http_server_listen;
    map["httpServerBindToAnyPort"] = http_server_bind_to_any_port;
    map["httpServerListenAfterBind"] = http_server_listen_after_bind;
    map["httpServerSetMountPoint"] = http_server_set_mount_point;
    map["httpServerRmMountPoint"] = http_server_remove_mount_point;
    map["httpServerSetFileExtAndMimetypeMapping"] = http_server_set_fext_mimetype;
    map["httpServerEnableLogger"] = http_server_enable_logger;

    map["httpCreateHeaders"] = http_create_headers;
    map["httpCreateClient"] = http_create_client;
    map["httpClientGet"] = http_client_get;
    map["httpCreateParams"] = http_create_params;
    map["httpClientPost"] = http_client_post;
    map["httpClientPut"] = http_client_put;
    map["httpClientDelete"] = http_client_delete;
    map["httpClientSetFollowLocation"] = http_client_set_follow_location;

    return map;
}