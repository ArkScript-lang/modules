#include <Ark/Module.hpp>
#include <bitset>

namespace ArkBitwise
{
    Value rshift(std::vector<Value>& n, Ark::VM* vm)
    {
        if (n.size() != 2)
            throw std::runtime_error("bitwise:rshift: needs  2 arguments, number and shift");
        if (n[0].valueType() != ValueType::Number 
            && n[1].valueType() != ValueType::Number)
            throw Ark::TypeError("bitwise:rshift the 2 arguments must be numbers"); 

        long num = static_cast<long>(n[0].number());
        long shift = static_cast<long>(n[1].number());
        long shifted = num >> shift;
        return Value(shifted);
    }

    Value lshift(std::vector<Value>& n, Ark::VM* vm)
    {
        if (n.size() != 2)
            throw std::runtime_error("bitwise:lshift: needs  2 arguments, number and shift");
        if (n[0].valueType() != ValueType::Number 
            && n[1].valueType() != ValueType::Number)
            throw Ark::TypeError("bitwise:lshift the 2 arguments must be numbers"); 

        long num = static_cast<long>(n[0].number());
        long shift = static_cast<long>(n[1].number());
        long shifted = num << shift;
        return Value(shifted);
    }
    
    Value xor_(std::vector<Value>& n, Ark::VM* vm)
    {
        if (n.size() != 2)
            throw std::runtime_error("bitwise:xor: needs  2 numbers as arguments, number and mask");
        if (n[0].valueType() != ValueType::Number 
            && n[1].valueType() != ValueType::Number)
            throw Ark::TypeError("bitwise:xor the 2 arguments must be numbers"); 

        long num = static_cast<long>(n[0].number());
        long mask = static_cast<long>(n[1].number());
        long masked = num ^ mask;
        return Value(masked);
    }

    Value or_(std::vector<Value>& n, Ark::VM* vm)
    {
        if (n.size() != 2)
            throw std::runtime_error("bitwise:or : needs  2 numbers as arguments, number and mask");
        if (n[0].valueType() != ValueType::Number 
            && n[1].valueType() != ValueType::Number)
            throw Ark::TypeError("bitwise:or the 2 arguments must be numbers"); 

        long num = static_cast<long>(n[0].number());
        long mask = static_cast<long>(n[1].number());
        long masked = num | mask;
        return Value(masked);
    }

    Value and_(std::vector<Value>& n, Ark::VM* vm)
    {
        if (n.size() != 2)
            throw std::runtime_error("bitwise:and : needs  2 numbers as arguments, number and mask");
        if (n[0].valueType() != ValueType::Number 
            && n[1].valueType() != ValueType::Number)
            throw Ark::TypeError("bitwise:and the 2 arguments must be numbers"); 

        long num = static_cast<long>(n[0].number());
        long mask = static_cast<long>(n[1].number());
        long masked = num & mask;
        return Value(masked);
    }
}

ARK_API_EXPORT mapping *getFunctionsMapping()
{
    mapping *map = mapping_create(5);
    mapping_add(map[0], "bitwise:rshift", ArkBitwise::rshift);
    mapping_add(map[1], "bitwise:lshift", ArkBitwise::lshift);
    mapping_add(map[2], "bitwise:xor", ArkBitwise::xor_);
    mapping_add(map[3], "bitwise:or", ArkBitwise::or_);
    mapping_add(map[4], "bitwise:and", ArkBitwise::and_);

    return map;
}