#ifndef ARK_MSGPACK_OBJ_HPP
#define ARK_MSGPACK_OBJ_HPP

#include <variant>
/*
#include <iostream>
#include <sstream>
#include <msgpack.hpp>
*/
#include <Ark/Module.hpp>

using CObject = std::variant<bool, double, std::string, std::vector<Value>>;
extern CObject get_cobject(Value& ark_object, ValueType type);

extern Value msgpack_obj_handle(std::vector<Value>& args, VM* vm);
extern Value msgpack_sbuffer(std::vector<Value>& args, VM* vm);
extern Value msgpack_obj(std::vector<Value>& args, VM* vm);

#endif