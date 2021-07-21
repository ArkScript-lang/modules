#include <ark_msgpack.hpp>

ARK_API_EXPORT mapping* getFunctionsMapping()
{
    mapping* map = mapping_create(6);

    // msgpack objects constructors
    mapping_add(map[0], "msgpack:sbuffer", msgpack_sbuffer);
    mapping_add(map[1], "msgpack:objectHandle", msgpack_obj_handle);
    mapping_add(map[2], "msgpack:object", msgpack_obj);

    // msgpack operations
    mapping_add(map[3], "msgpack:pack", msgpack_pack);
    mapping_add(map[4], "msgpack:unpack", msgpack_unpack);
    mapping_add(map[5], "msgpack:convert", msgpack_convert);

    return map;
}