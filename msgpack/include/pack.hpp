#ifndef ARK_MSGPACK_PACK_HPP
#define ARK_MSGPACK_PACK_HPP

#include <object.hpp>

namespace ArkMsgpack
{
	namespace Minimal
	{
		extern Value list_packing(std::vector<Value> &list, Ark::VM* vm);
    	extern Value pack(std::vector<Value> &args, Ark::VM* vm);
	}

	extern Value pack(std::vector<Value> &args, Ark::VM* vm);
}

#endif