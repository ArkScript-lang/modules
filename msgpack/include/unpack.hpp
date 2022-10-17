#ifndef ARK_MSGPACK_UNPACK_HPP
#define ARK_MSGPACK_UNPACK_HPP

#include <objects.hpp>

Ark::Value msgpack_unpack(std::vector<Ark::Value>& args, Ark::VM* vm);
Ark::Value msgpack_convert(std::vector<Ark::Value>& args, Ark::VM* vm);

#endif