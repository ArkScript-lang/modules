#include <ark_msgpack.hpp>

ARK_API Ark::mapping* getFunctionsMapping()
{
    static Ark::mapping map[] = {
        // msgpack objects constructors
        { "msgpack:sbuffer", msgpack_sbuffer },
        { "msgpack:objectHandle", msgpack_obj_handle },
        { "msgpack:object", msgpack_obj },

        // msgpack operations
        { "msgpack:pack", msgpack_pack },
        { "msgpack:unpack", msgpack_unpack },
        { "msgpack:convert", msgpack_convert },

        { nullptr, nullptr }
    };

    return map;
}