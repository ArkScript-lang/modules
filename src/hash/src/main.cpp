#include <picosha2.h>
#include <md5.hpp>

#include <Ark/Module.hpp>

namespace ArkHash
{
    using namespace Ark;

    Value sha2(std::vector<Value>& n, VM* vm [[maybe_unused]])
    {
        if (!types::check(n, ValueType::String))
            types::generateError(
                "hash:sha256",
                { { types::Contract { { types::Typedef("data", ValueType::String) } } } },
                n);

        std::string src = n[0].stringRef();
        std::string output;
        picosha2::hash256_hex_string(src, output);

        return Value(output);
    }

    Value md5(std::vector<Value>& n, VM* vm [[maybe_unused]])
    {
        if (!types::check(n, ValueType::String))
            types::generateError(
                "hash:md5",
                { { types::Contract { { types::Typedef("data", ValueType::String) } } } },
                n);

        return Value(md5::md5(n[0].stringRef()));
    }
}

ARK_API Ark::mapping* getFunctionsMapping()
{
    static Ark::mapping map[] = {
        { "hash:sha256", ArkHash::sha2 },
        { "hash:md5", ArkHash::md5 },
        { nullptr, nullptr }
    };

    return map;
}
