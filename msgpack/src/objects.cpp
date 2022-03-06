#include <objects.hpp>
#include <adaptor.hpp>

std::list<std::unique_ptr<msgpack::sbuffer>>& get_sbuffers()
{
    static std::list<std::unique_ptr<msgpack::sbuffer>> sbuffers;
    return sbuffers;
}

std::list<std::unique_ptr<msgpack::object_handle>>& get_obj_handlers()
{
    static std::list<std::unique_ptr<msgpack::object_handle>> object_handlers;
    return object_handlers;
}

std::list<std::unique_ptr<msgpack::object>>& get_objects()
{
    static std::list<std::unique_ptr<msgpack::object>> objects;
    return objects;
}

UserType::ControlFuncs* get_cfs_sbuffers()
{
    static UserType::ControlFuncs cfs;
    cfs.ostream_func = [](std::ostream& os, const UserType& a) -> std::ostream& {
        os << "msgpackSBuffer<" << a.data() << ">";
        return os;
    };

    cfs.deleter = [](void* data) {
        std::list<std::unique_ptr<msgpack::sbuffer>>& sbuffers = get_sbuffers();
        for (auto it = sbuffers.begin(), end = sbuffers.end(); it != end; ++it)
        {
            if (&(*it) == data)
            {
                sbuffers.erase(it);
                break;
            }
        }
    };

    return &cfs;
}

UserType::ControlFuncs* get_cfs_obj_handlers()
{
    static UserType::ControlFuncs cfs;
    cfs.ostream_func = [](std::ostream& os, const UserType& a) -> std::ostream& {
        os << "msgpackObjectHandler<" << a.data() << ">";
        return os;
    };

    cfs.deleter = [](void* data) {
        std::list<std::unique_ptr<msgpack::object_handle>>& object_handlers = get_obj_handlers();
        for (auto it = object_handlers.begin(), end = object_handlers.end(); it != end; ++it)
        {
            if (&(*it) == data)
            {
                object_handlers.erase(it);
                break;
            }
        }
    };

    return &cfs;
}

UserType::ControlFuncs* get_cfs_objects()
{
    static UserType::ControlFuncs cfs;
    cfs.ostream_func = [](std::ostream& os, const UserType& a) -> std::ostream& {
        os << "msgpackObject<" << a.data() << ">";
        return os;
    };

    cfs.deleter = [](void* data) {
        std::list<std::unique_ptr<msgpack::object>>& objects = get_objects();
        for (auto it = objects.begin(), end = objects.end(); it != end; ++it)
        {
            if (&(*it) == data)
            {
                objects.erase(it);
                break;
            }
        }
    };

    return &cfs;
}

CObject get_cobject(Value& ark_object, ValueType type)
{
    CObject object;

    switch (type)
    {
        case ValueType::True:
            object = true;
            break;
        case ValueType::False:
            object = false;
            break;
        case ValueType::Number:
            object = ark_object.number();
            break;
        case ValueType::String:
            object = ark_object.stringRef();
            break;
        case ValueType::List:
            object = ark_object.list();
            break;
        // The following types deliberately have no action.
        case ValueType::PageAddr:
        case ValueType::CProc:
        case ValueType::Closure:
        case ValueType::User:
        case ValueType::Nil:
        case ValueType::Undefined:
        case ValueType::Reference:
        case ValueType::InstPtr:
            break;
            // Note: All values must be listed to prevent issues when a new
            //       member of ValueType is added so that compiler can detect
            //       the missing type and warn maintainer.
    }

    return object;
}

Value msgpack_sbuffer(std::vector<Value>& args [[maybe_unused]], VM* vm [[maybe_unused]])
{
    return Value(UserType(get_sbuffers().emplace_back(std::make_unique<msgpack::sbuffer>()).get(), get_cfs_sbuffers()));
}

Value msgpack_obj_handle(std::vector<Value>& args [[maybe_unused]], VM* vm [[maybe_unused]])
{
    return Value(UserType(get_obj_handlers().emplace_back(std::make_unique<msgpack::object_handle>()).get(), get_cfs_obj_handlers()));
}

Value msgpack_obj(std::vector<Value>& args, VM* vm [[maybe_unused]])
{
    if (!types::check(args, ValueType::User) || !args[0].usertype().is<msgpack::object_handle>())
        types::generateError(
            "msgpack:object",
            { { types::Contract { { types::Typedef("msgpackObject", ValueType::User) } } } },
            args);

    msgpack::object_handle& oh = args[0].usertypeRef().as<msgpack::object_handle>();
    return Value(UserType(get_objects().emplace_back(std::make_unique<msgpack::object>(oh.get())).get(), get_cfs_objects()));
}
