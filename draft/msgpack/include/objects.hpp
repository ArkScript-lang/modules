#ifndef ARK_MSGPACK_OBJ_HPP
#define ARK_MSGPACK_OBJ_HPP

#include <variant>
#include <memory>
#include <msgpack.hpp>

#include <Ark/Module.hpp>


using CObject = std::variant<bool, double, std::string, std::vector<Ark::Value>>;

std::list<std::unique_ptr<msgpack::sbuffer>>& get_sbuffers();
std::list<std::unique_ptr<msgpack::object_handle>>& get_obj_handlers();
std::list<std::unique_ptr<msgpack::object>>& get_objects();
Ark::UserType::ControlFuncs* get_cfs_sbuffers();
Ark::UserType::ControlFuncs* get_cfs_obj_handlers();
Ark::UserType::ControlFuncs* get_cfs_objects();
CObject get_cobject(Ark::Value& ark_object, Ark::ValueType type);

Ark::Value msgpack_obj_handle(std::vector<Ark::Value>& args, Ark::VM* vm);
Ark::Value msgpack_sbuffer(std::vector<Ark::Value>& args, Ark::VM* vm);
Ark::Value msgpack_obj(std::vector<Ark::Value>& args, Ark::VM* vm);

#endif