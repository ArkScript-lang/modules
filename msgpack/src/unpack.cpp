#include <unpack.hpp>
#include <adaptor.hpp>

Value msgpack_unpack(std::vector<Value>& args, VM* vm [[maybe_unused]])
{
    if(args.size() != 2)
        throw std::runtime_error("msgpack:unpack require two arguments, first a msgpack:sbuffer and last  msgpack:object_handle");
    if(args[0].valueType() != ValueType::User || !(args[0].usertype().is<msgpack::sbuffer>()))
        throw Ark::TypeError("msgpack:unpack: packed buffer must be msgpack:sbuffer");
    if(args[1].valueType() != ValueType::User || !(args[1].usertype().is<msgpack::object_handle>()))
        throw Ark::TypeError("msgpack:unpack: packed buffer must be msgpack:object_handle");

    msgpack::sbuffer& sbuf = args[0].usertypeRef().as<msgpack::sbuffer>();
    msgpack::object_handle& oh = args[1].usertypeRef().as<msgpack::object_handle>();

    oh = msgpack::unpack(sbuf.data(), sbuf.size());

    return Value(Ark::Nil);
}

Value msgpack_convert(std::vector<Value>& args, VM* vm [[maybe_unused]])
{
    if(args.size() != 1)
        throw std::runtime_error("msgpack:convert requires one argument, a msgpack:object to convert to a usable ark object");
    if(args[0].valueType() != ValueType::User || !(args[0].usertype().is<msgpack::object>()))
        throw Ark::TypeError("msgpack:convert: packed buffer must be msgpack:object");

    msgpack::object& o = args[0].usertypeRef().as<msgpack::object>();
    Value dst;

    o.convert(dst);

    return dst;
}
