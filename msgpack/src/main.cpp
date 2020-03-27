#include <ark_msgpack.hpp>

ARK_API_EXPORT Mapping_t getFunctionsMapping()
{
    Mapping_t map;

    map["msgpackMiniPack"] = ArkMsgpack::Minimal::pack;
    map["msgpackMiniUnpack"] = ArkMsgpack::Minimal::unpack;
    map["msgpackMiniObjStr"] = ArkMsgpack::Minimal::object_str;
    // msgpack objects constructors
    map["msgpackObjHandle"] = ArkMsgpack::msgpack_obj_handle;
    return map;
}