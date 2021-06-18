#include <Ark/Module.hpp>
#include <bitset>

namespace ArkBitwise
{

    std::vector<std::unique_ptr<std::bitset<8>>>& getBitwiseObject()
    {
        static std::vector<std::unique_ptr<std::bitset<8>>> bitsetObject;
        return bitsetObject;
    }

    Ark::Value convert2Bitset(unsigned long long val)
    {
        std::bitset<8>* ptr = getBitwiseObject().emplace_back(std::make_unique<std::bitset<8>>(val)).get();
        Value v = Ark::Value(Ark::UserType(ptr));
        return v;
    }

    Ark::Value convert2Bitset(std::string val,
                                   unsigned long long pos = NULL,
                                   unsigned long long len = NULL)
    {
        std::bitset<8>* ptr = getBitwiseObject().emplace_back(std::make_unique<std::bitset<8>>(val, pos, len)).get();
        Value v = Ark::Value(Ark::UserType(ptr));
        return v;
    }

    Ark::Value makeBitset(std::vector<Value> &n, Ark::VM *vm)
    {
        if (n.size() < 1)
            throw std::runtime_error("bitwise:makeBitset need at least a single argument: data");
        Ark::Value v;
        ValueType nType = n[0].valueType();
        if (nType != ValueType::String || nType != ValueType::Number)
            throw std::runtime_error("bitwise:makeBitset: the first argument can only be a String or a Number");

        if (nType == ValueType::Number)
        {
            unsigned long long num = static_cast<long>(n[0].number());
            v = Ark::Value(Ark::UserType(&convert2Bitset(num)));
        }
        else if (nType == ValueType::String)
        {

            std::string src = n[0].stringRef().toString();
            unsigned long long pos, len;
            try
            {
                if (n.size() == 2 && n[1].valueType() == ValueType::Number)
                {
                    pos = static_cast<long>(n[1].number());
                    v = convert2Bitset(src, pos);
                }
                else if (n.size() == 3 && n[1].valueType() == ValueType::Number && n[2].valueType() == ValueType::Number)
                {
                    pos = static_cast<long>(n[1].number());
                    len = static_cast<long>(n[2].number());
                    v = convert2Bitset(src, pos, len);
                }

                return v;
            }
            catch (const std::out_of_range e)
            {
                throw std::runtime_error("bitwise:makeBitset: ");
            }
        }

        return Ark::Nil;
    }

    Value rshift(std::vector<Value> &n, Ark::VM *vm)
    {
        return Ark::Nil;
    }

}

ARK_API_EXPORT mapping *getFunctionsMapping()
{
    mapping *map = mapping_create(2);
    /*
    mapping_add(map[0], "hash:sha256", ArkHash::sha2);
    mapping_add(map[1], "hash:md5", ArkHash::md5);
    */
    return map;
}