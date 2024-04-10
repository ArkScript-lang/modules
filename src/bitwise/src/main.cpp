#include <Ark/Module.hpp>
#include <bitset>

using namespace Ark;

namespace ArkBitwise
{
    Value rshift(std::vector<Value>& n, VM* vm [[maybe_unused]])
    {
        if (!types::check(n, ValueType::Number, ValueType::Number))
            types::generateError(
                "bitwise:rshift",
                { { types::Contract { { types::Typedef("number", ValueType::Number), types::Typedef("shift", ValueType::Number) } } } },
                n);

        long num = static_cast<long>(n[0].number());
        long shift = static_cast<long>(n[1].number());
        long shifted = num >> shift;
        return Value(static_cast<double>(shifted));
    }

    Value lshift(std::vector<Value>& n, VM* vm [[maybe_unused]])
    {
        if (!types::check(n, ValueType::Number, ValueType::Number))
            types::generateError(
                "bitwise:lshift",
                { { types::Contract { { types::Typedef("number", ValueType::Number), types::Typedef("shift", ValueType::Number) } } } },
                n);

        long num = static_cast<long>(n[0].number());
        long shift = static_cast<long>(n[1].number());
        long shifted = num << shift;
        return Value(static_cast<double>(shifted));
    }

    Value xor_(std::vector<Value>& n, VM* vm [[maybe_unused]])
    {
        if (!types::check(n, ValueType::Number, ValueType::Number))
            types::generateError(
                "bitwise:xor",
                { { types::Contract { { types::Typedef("number", ValueType::Number), types::Typedef("mask", ValueType::Number) } } } },
                n);

        long num = static_cast<long>(n[0].number());
        long mask = static_cast<long>(n[1].number());
        long masked = num ^ mask;
        return Value(static_cast<double>(masked));
    }

    Value or_(std::vector<Value>& n, VM* vm [[maybe_unused]])
    {
        if (!types::check(n, ValueType::Number, ValueType::Number))
            types::generateError(
                "bitwise:or",
                { { types::Contract { { types::Typedef("a", ValueType::Number), types::Typedef("b", ValueType::Number) } } } },
                n);

        long num = static_cast<long>(n[0].number());
        long mask = static_cast<long>(n[1].number());
        long masked = num | mask;
        return Value(static_cast<double>(masked));
    }

    Value and_(std::vector<Value>& n, VM* vm [[maybe_unused]])
    {
        if (!types::check(n, ValueType::Number, ValueType::Number))
            types::generateError(
                "bitwise:and",
                { { types::Contract { { types::Typedef("a", ValueType::Number), types::Typedef("b", ValueType::Number) } } } },
                n);

        long num = static_cast<long>(n[0].number());
        long mask = static_cast<long>(n[1].number());
        long masked = num & mask;
        return Value(static_cast<double>(masked));
    }
}

ARK_API mapping* getFunctionsMapping()
{
    static mapping map[] = {
        { "bitwise:rshift", ArkBitwise::rshift },
        { "bitwise:lshift", ArkBitwise::lshift },
        { "bitwise:xor", ArkBitwise::xor_ },
        { "bitwise:or", ArkBitwise::or_ },
        { "bitwise:and", ArkBitwise::and_ },
        { nullptr, nullptr }
    };

    return map;
}
