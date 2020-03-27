#include <ark_msgpack.hpp>

ARK_API_EXPORT Mapping_t getFunctionsMapping()
{
    Mapping_t map;

    map["msgMiniPack"] = ArkMsgpack::Minimal::pack;
    map["msgMiniUnpack"] = ArkMsgpack::Minimal::unpack;
    map["msgMiniObjStr"] = ArkMsgpack::Minimal::object_str;
    // msgpack objects constructors
    map["msgObjHandle"] = ArkMsgpack::msgpack_obj_handle;
    return map;
}