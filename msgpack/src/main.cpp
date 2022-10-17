#include <ark_msgpack.hpp>

ARK_API Ark::mapping* getFunctionsMapping()
{
    Ark::mapping* map = Ark::mapping_create(6);

    // msgpack objects constructors
    Ark::mapping_add(map[0], "msgpack:sbuffer", msgpack_sbuffer);
    Ark::mapping_add(map[1], "msgpack:objectHandle", msgpack_obj_handle);
    Ark::mapping_add(map[2], "msgpack:object", msgpack_obj);

    // msgpack operations
    Ark::mapping_add(map[3], "msgpack:pack", msgpack_pack);
    Ark::mapping_add(map[4], "msgpack:unpack", msgpack_unpack);
    Ark::mapping_add(map[5], "msgpack:convert", msgpack_convert);

    return map;
}