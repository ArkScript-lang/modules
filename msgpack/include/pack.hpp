#ifndef ARK_MSGPACK_PACK_HPP
#define ARK_MSGPACK_PACK_HPP

#include <object.hpp>

namespace ArkMsgpack
{
	extern Value pack(std::vector<Value> &args, VM* vm);
}

#endif