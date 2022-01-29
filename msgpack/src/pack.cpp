#include <pack.hpp>
#include <adaptor.hpp>


Value msgpack_pack(std::vector<Value>& args, VM* vm [[maybe_unused]])
{
    if (args.size() != 2)
        throw std::runtime_error("msgpack:pack require two arguments, first the msgpack:sbuffer and last an ark object");
    if (args[0].valueType() != ValueType::User || !(args[0].usertype().is<msgpack::sbuffer>()))
        throw Ark::TypeError("msgpack:pack: packed buffer must be a msgpack:sbuffer");

    msgpack::sbuffer& sbuf = args[0].usertypeRef().as<msgpack::sbuffer>();

    msgpack::pack(sbuf, args[1]);

    return Ark::Nil;
}
