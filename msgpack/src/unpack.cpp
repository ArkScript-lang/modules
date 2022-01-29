#include <unpack.hpp>
#include <adaptor.hpp>

Value msgpack_unpack(std::vector<Value>& args, VM* vm [[maybe_unused]])
{
    if (!types::check(args, ValueType::User, ValueType::User) || !args[0].usertypeRef().is<msgpack::sbuffer>() || !args[1].usertypeRef().is<msgpack::object_handle>())
        types::generateError(
            "msgpack:unpack",
            { { types::Contract { { types::Typedef("msgpackBuffer", ValueType::User),
                                    types::Typedef("msgpackObject", ValueType::User) } } } },
            args);

    msgpack::sbuffer& sbuf = args[0].usertypeRef().as<msgpack::sbuffer>();
    msgpack::object_handle& oh = args[1].usertypeRef().as<msgpack::object_handle>();

    oh = msgpack::unpack(sbuf.data(), sbuf.size());

    return Value(Ark::Nil);
}

Value msgpack_convert(std::vector<Value>& args, VM* vm [[maybe_unused]])
{
    if (!types::check(args, ValueType::User) || !args[0].usertype().is<msgpack::object>())
        types::generateError(
            "msgpack:convert",
            { { types::Contract { { types::Typedef("msgpackObject", ValueType::User) } } } },
            args);

    msgpack::object& o = args[0].usertypeRef().as<msgpack::object>();
    Value dst;
    o.convert(dst);

    return dst;
}
