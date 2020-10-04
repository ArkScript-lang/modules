#ifndef ARK_MSGPACK_UNPACK_HPP
#define ARK_MSGPACK_UNPACK_HPP

#include <object.hpp>

namespace ArkMsgpack
{
	namespace Minimal
	{
    	extern Value list_unpacking(std::vector<Value> &buffer_list, Ark::VM* vm);
    	extern void list_unpacked_str(std::vector<Value> &buffer_list, std::ostringstream &stream, Ark::VM* vm);
    	extern Value unpack(std::vector<Value> &args, Ark::VM* vm);
    	extern Value object_str(std::vector<Value> &args, Ark::VM* vm);
	}

	extern Value unpack(std::vector<Value> &args, Ark::VM* vm);
	extern Value convert(std::vector<Value> &args, Ark::VM* vm);
}

#endif