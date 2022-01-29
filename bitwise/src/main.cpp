#include <Ark/Module.hpp>
#include <bitset>

namespace ArkBitwise
{
    Value rshift(std::vector<Value>& n, Ark::VM* vm [[maybe_unused]])
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

    Value lshift(std::vector<Value>& n, Ark::VM* vm [[maybe_unused]])
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

    Value xor_(std::vector<Value>& n, Ark::VM* vm [[maybe_unused]])
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

    Value or_(std::vector<Value>& n, Ark::VM* vm [[maybe_unused]])
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

    Value and_(std::vector<Value>& n, Ark::VM* vm [[maybe_unused]])
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
    mapping* map = mapping_create(5);
    mapping_add(map[0], "bitwise:rshift", ArkBitwise::rshift);
    mapping_add(map[1], "bitwise:lshift", ArkBitwise::lshift);
    mapping_add(map[2], "bitwise:xor", ArkBitwise::xor_);
    mapping_add(map[3], "bitwise:or", ArkBitwise::or_);
    mapping_add(map[4], "bitwise:and", ArkBitwise::and_);

    return map;
}
