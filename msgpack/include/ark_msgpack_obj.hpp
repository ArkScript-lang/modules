#ifndef ARK_MSGPACK_OBJ_HPP
#define ARK_MSGPACK_OBJ_HPP

#include <variant>
#include <iostream> 
#include <sstream>
#include <msgpack.hpp>
#include <Ark/Module.hpp>

using CObject = std::variant<bool, double, std::string, std::vector<Value>>;
extern CObject get_cobject(const Value &ark_objekt, ValueType type);

#endif