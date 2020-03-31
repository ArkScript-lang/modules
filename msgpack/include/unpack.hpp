#ifndef ARK_MSGPACK_UNPACK_HPP
#define ARK_MSGPACK_UNPACK_HPP

#include <object.hpp>

namespace ArkMsgpack
{
	namespace Minimal
	{
    	extern Value list_unpacking(std::vector<Value> &buffer_list);
    	extern void list_unpacked_str(std::vector<Value> &buffer_list, std::ostringstream &stream);
    	extern Value unpack(std::vector<Value> &args);
    	extern Value object_str(std::vector<Value> &args);
	}

	extern Value unpack(std::vector<Value> &args);
	extern Value convert(std::vector<Value> &args);
}

#endif