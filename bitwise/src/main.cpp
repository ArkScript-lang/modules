#include <Ark/Module.hpp>
#include <bitset>

namespace ArkBitwise
{
    Value rshift(std::vector<Value>& n, Ark::VM* vm)
    {

        if (n.size() != 2)
            throw std::runtime_error("bitwise:rshift: needs 1 argument: number");
        if (n[0].valueType() != ValueType::Number 
            && n[1].valueType() != ValueType::Number)
            throw Ark::TypeError("bitwise:rshift the 2 arguments must be numbers"); 

        int num = static_cast<int>(n[0].number());
        int shift = static_cast<int>(n[1].number());
        int shifted = num >> shift;
        return Value(shifted);
    }

    Value lshift(std::vector<Value>& n, Ark::VM* vm)
    {
   
        if (n.size() != 2)
            throw std::runtime_error("bitwise:lshift: needs 1 argument: number");
        if (n[0].valueType() != ValueType::Number 
            && n[1].valueType() != ValueType::Number)
            throw Ark::TypeError("bitwise:lshift the 2 arguments must be numbers"); 

        int num = static_cast<int>(n[0].number());
        int shift = static_cast<int>(n[1].number());
        int shifted = num << shift;
        return Value(shifted);
    }
    
    Value xor(std::vector<Value>& n, Ark::VM* vm)
    {

        if (n.size() != 2)
            throw std::runtime_error("bitwise:xor: needs 1 argument: number");
        if (n[0].valueType() != ValueType::Number 
            && n[1].valueType() != ValueType::Number)
            throw Ark::TypeError("bitwise:xor the 2 arguments must be numbers"); 

        int num = static_cast<int>(n[0].number());
        int shift = static_cast<int>(n[1].number());
        int shifted = num ^ shift;
        return Value(shifted);
    }
}

ARK_API_EXPORT mapping *getFunctionsMapping()
{
    mapping *map = mapping_create(3);
    mapping_add(map[0], "bitwise:rshift", ArkBitwise::rshift);
    mapping_add(map[1], "bitwise:lshift", ArkBitwise::lshift);
    mapping_add(map[2], "bitwise:xor", ArkBitwise::xor);
    return map;
}