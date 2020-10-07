#ifndef ARK_MSGPACK_UNPACK_HPP
#define ARK_MSGPACK_UNPACK_HPP

#include <object.hpp>

namespace ArkMsgpack
{
	extern Value unpack(std::vector<Value> &args, VM* vm);
	extern Value convert(std::vector<Value> &args, VM* vm);
}

#endif