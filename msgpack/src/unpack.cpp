#include <unpack.hpp>
#include <adaptor.hpp>

Value msgpack_unpack(std::vector<Value>& args, VM* vm)
{
    if(args.size() != 2)
        throw std::runtime_error("ArgError : This function must have 2 arguments");
    if(args[0].valueType() != ValueType::User || !(args[0].usertype().is<msgpack::sbuffer>()))
        throw Ark::TypeError("msgpack:unpack: packed buffer must be msgpack::sbuffer");
    if(args[1].valueType() != ValueType::User || !(args[1].usertype().is<msgpack::object_handle>()))
        throw Ark::TypeError("msgpack:unpack: packed buffer must be msgpack::object_handle");

    msgpack::sbuffer& sbuf = args[0].usertypeRef().as<msgpack::sbuffer>();
    msgpack::object_handle& oh = args[1].usertypeRef().as<msgpack::object_handle>();

    oh = msgpack::unpack(sbuf.data(), sbuf.size());

    return Value(Ark::Nil);
}

Value msgpack_convert(std::vector<Value>& args, VM* vm)
{
    if(args.size() != 1)
        throw std::runtime_error("ArgError: this function must have 1 arguments");
    if(args[0].valueType() != ValueType::User || !(args[0].usertype().is<msgpack::object>()))
        throw Ark::TypeError("msgpack:convert: packed buffer must be msgpack::object");

    msgpack::object& o = args[0].usertypeRef().as<msgpack::object>();
    Value dst;

    o.convert(dst);

    return dst;
}
