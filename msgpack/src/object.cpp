#include <object.hpp>
#include <adaptor.hpp>

CObject get_cobject(const Value &ark_object, ValueType type)
{
    CObject object;

    switch(type)
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
            object = const_cast<Value&>(ark_object).stringRef().toString();
            break;
        case ValueType::List:
            object = const_cast<Value&>(ark_object).list();
            break;
    }

    return object;
}

// msgpack::sbuffer
Value msgpack_sbuffer(std::vector<Value> &args, VM* vm)
{
    if(args.size() != 0)
        throw std::runtime_error("ArgError : No arguments necessary for this function");

    ValueManager::newObject<msgpack::sbuffer>();
    std::cout << "size : " << ValueManager::container.m_container.size() << "\n";
    msgpack::sbuffer* t = static_cast<msgpack::sbuffer*>(ValueManager::container.getBackObject());
    *t = msgpack::sbuffer();

    return Value(UserType(t));
}

// msgpack::object_handle and msgpack::object
Value msgpack_obj_handle(std::vector<Value> &args, VM* vm)
{
    /*if(args.size() != 1)
        throw std::runtime_error("ArgError : This function must have 1 argument");
    if(args[0].valueType() != ValueType::User || !(args[0].usertype().is<msgpack::sbuffer>()))
        throw Ark::TypeError("msgpack:objectHandle The packed buffer must be a msgpack::sbuffer");*/

    ValueManager::newObject<msgpack::object_handle>();
    std::cout << "size : " << ValueManager::container.m_container.size() << "\n";
    auto* t = static_cast<msgpack::object_handle*>(ValueManager::container.getBackObject());
    *t = msgpack::object_handle();

    return Value(UserType(t));
}

Value msgpack_obj(std::vector<Value> &args, VM* vm)
{
    if(args.size() != 1)
        throw std::runtime_error("ArgError : This function must have 1 argument");
    if(args[0].valueType() != ValueType::User || !(args[0].usertype().is<msgpack::object_handle>()))
        throw Ark::TypeError("msgpack:object The packed buffer must be a msgpack::object_handle");

    msgpack::object* o;

    ValueManager::newObject<msgpack::object>();
    o = static_cast<msgpack::object*>(ValueManager::container.getBackObject());
    msgpack::object_handle& oh = args[0].usertypeRef().as<msgpack::object_handle>();
    *o = oh.get();

    return Value(UserType(o));
}
