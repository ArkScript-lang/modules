#ifndef ARK_MSGPACK_PACK_HPP
#define ARK_MSGPACK_PACK_HPP

#include <ark_msgpack_obj.hpp>

namespace ArkMsgpack
{
	namespace Minimal
	{
		extern Value list_packing(std::vector<Value> &list);
    	extern Value pack(std::vector<Value> &args);
	}
}

#endif