#ifndef ARK_MSGPACK_UNPACK_HPP
#define ARK_MSGPACK_UNPACK_HPP

#include <object.hpp>

Value msgpack_unpack(std::vector<Value>& args, VM* vm);
Value msgpack_convert(std::vector<Value>& args, VM* vm);

#endif