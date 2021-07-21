#ifndef ARK_MSGPACK_OBJ_HPP
#define ARK_MSGPACK_OBJ_HPP

#include <variant>
#include <memory>
#include <msgpack.hpp>

#include <Ark/Module.hpp>


using CObject = std::variant<bool, double, std::string, std::vector<Value>>;

std::list<std::unique_ptr<msgpack::sbuffer>>& get_sbuffers();
std::list<std::unique_ptr<msgpack::object_handle>>& get_obj_handlers();
std::list<std::unique_ptr<msgpack::object>>& get_objects();
UserType::ControlFuncs* get_cfs_sbuffers();
UserType::ControlFuncs* get_cfs_obj_handlers();
UserType::ControlFuncs* get_cfs_objects();
CObject get_cobject(Value& ark_object, ValueType type);

Value msgpack_obj_handle(std::vector<Value>& args, VM* vm);
Value msgpack_sbuffer(std::vector<Value>& args, VM* vm);
Value msgpack_obj(std::vector<Value>& args, VM* vm);

#endif