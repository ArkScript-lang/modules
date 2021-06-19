#include <ark_msgpack.hpp>

ARK_API_EXPORT Mapping_t getFunctionsMapping()
{
    mapping* map = mapping_create(6);

    // msgpack objects constructors
    map["msgpack:sbuffer"] = msgpack_sbuffer;
    map["msgpack:objectHandle"] = msgpack_obj_handle;
    map["msgpack:object"] = msgpack_obj;

    // msgpack operations
    map["msgpack:pack"] = msgpack_pack;
    map["msgpack:unpack"] = msgpack_unpack;
    map["msgpack:convert"] = msgpack_convert;

    return map;
}
