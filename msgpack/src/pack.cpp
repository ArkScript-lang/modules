#include <pack.hpp>
#include <adaptor.hpp>

using namespace Ark;

Value msgpack_pack(std::vector<Value>& args, VM* vm [[maybe_unused]])
{
    if (!types::check(args, ValueType::User, ValueType::Any) || !args[0].usertypeRef().is<msgpack::sbuffer>())
        types::generateError(
            "msgpack:pack",
            { { types::Contract { { types::Typedef("msgpackBuffer", ValueType::User),
                                    types::Typedef("arkValue", ValueType::Any) } } } },
            args);

    msgpack::sbuffer& sbuf = args[0].usertypeRef().as<msgpack::sbuffer>();
    msgpack::pack(sbuf, args[1]);

    return Nil;
}
