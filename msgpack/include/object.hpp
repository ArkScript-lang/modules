#ifndef ARK_MSGPACK_OBJ_HPP
#define ARK_MSGPACK_OBJ_HPP

#include <variant>
#include <iostream>
#include <sstream>
#include <msgpack.hpp>
#include <Ark/Module.hpp>

using CObject = std::variant<bool, double, std::string, std::vector<Value>>;
extern CObject get_cobject(const Value &ark_object, ValueType type);
namespace ArkMsgpack
{
	extern Value msgpack_obj_handle(std::vector<Value> &args);
	extern Value msgpack_sbuffer(std::vector<Value> &args);
	extern Value msgpack_obj(std::vector<Value> &args);
	extern Value print_msgpack_o(std::vector<Value> &args);
}

#endif