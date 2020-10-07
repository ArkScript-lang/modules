#include <ark_msgpack.hpp>

ARK_API_EXPORT Mapping_t getFunctionsMapping()
{
	Mapping_t map;

	/* Msgpack */
	// msgpack objects constructors
	map["msgpackSBuffer"] = ArkMsgpack::msgpack_sbuffer;
	map["msgpackObjHandle"] = ArkMsgpack::msgpack_obj_handle;
	map["msgpackObj"] = ArkMsgpack::msgpack_obj;

	// msgpack objets functions
	//map["printO"] = ArkMsgpack::print_msgpack_o;
	map["msgpackPack"] = ArkMsgpack::pack;
	map["msgpackUnpack"] = ArkMsgpack::unpack;
	map["msgpackConvert"] = ArkMsgpack::convert;

	return map;
}
