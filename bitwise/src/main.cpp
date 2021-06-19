#include <Ark/Module.hpp>
#include <bitset>

namespace ArkBitwise
{

    std::vector<std::unique_ptr<std::bitset<8>>>& getBitwiseObject()
    {
        static std::vector<std::unique_ptr<std::bitset<8>>> bitsetObject;
        return bitsetObject;
    }

    UserType::ControlFuncs* getCfs()
    {
        static UserType::ControlFuncs cfs;
        cfs.ostream_func = [](std::ostream& os, const UserType& a) -> std::ostream& {
            os << a.as<std::bitset<8>>();
            return os;
        };
        cfs.deleter = [](void* data) {
            std::vector<std::unique_ptr<std::bitset<8>>>& bitsets = getBitwiseObject();
            auto it = std::find_if(bitsets.begin(), bitsets.end(), [data](const auto& val) -> bool {
                return val.get() == static_cast<std::bitset<8>*>(data);
            });
            if (it != bitsets.end())
                bitsets.erase(it);
        };
        return &cfs;
    }

    Ark::Value bitset2Ark(std::bitset<8>& bitset)
    {
        std::bitset<8>* ptr = getBitwiseObject().emplace_back(std::make_unique<std::bitset<8>>(bitset)).get();
        Value v = Ark::Value(Ark::UserType(ptr));
        v.usertypeRef().setControlFuncs(getCfs());
        return v;
    }

    Ark::Value convert2Bitset(unsigned long long val)
    {
        std::bitset<8>* ptr = getBitwiseObject().emplace_back(std::make_unique<std::bitset<8>>(val)).get();
        Value v = Ark::Value(Ark::UserType(ptr));
        v.usertypeRef().setControlFuncs(getCfs());
        return v;
    }

    Ark::Value convert2Bitset(std::string val,
                                   unsigned long long pos = NULL,
                                   unsigned long long len = NULL)
    {
        std::bitset<8>* ptr = getBitwiseObject().emplace_back(std::make_unique<std::bitset<8>>(val, pos, len)).get();
        Value v = Ark::Value(Ark::UserType(ptr));
        v.usertypeRef().setControlFuncs(getCfs());
        return v;
    }

    Ark::Value makeBitset(std::vector<Value>& n, Ark::VM* vm)
    {
        if (n.size() < 1)
            throw std::runtime_error("bitwise:makeBitset need at least a single argument: data");
        else if (n.size() > 3)
            throw std::runtime_error("bitwise:makeBitset can only take up to 3 arguments");
        Ark::Value v;
        ValueType nType = n[0].valueType();
        /*
        if (nType != ValueType::String && nType != ValueType::Number)
            throw std::runtime_error("bitwise:makeBitset: the first argument can only be a String or a Number");
*/
        if (nType == ValueType::Number)
        {
            unsigned long long num = static_cast<long>(n[0].number());
            v  = convert2Bitset(num);
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
            }
            catch (const std::out_of_range e)
            {
                throw std::runtime_error("bitwise:makeBitset: ");
            }
        } 
        else
        {
            return Ark::Nil;
        }
        return v;
    }

    Value rshift(std::vector<Value>& n, Ark::VM* vm)
    {
        if (n.size() != 2)
            throw std::runtime_error("bitwise:rshift need at least 2 arguments: bitset, pos");

        if (n[0].valueType() != ValueType::User ||
                !n[0].usertype().is<std::bitset<8>>() )
            throw std::runtime_error("bitwise:rshift the first argument can only be a bitset");

        if (n[1].valueType() != ValueType::Number)
            throw std::runtime_error("bitwise:rshift the second argument can only be a Number");

        std::bitset<8>& bitsetObj = n[0].usertypeRef().as<std::bitset<8>>();
        std::bitset<8>& shifted = bitsetObj >> n[1].number();
        return bitset2Ark(shifted);
    }

    Value lshift(std::vector<Value>& n, Ark::VM* vm)
    {
        if (n.size() != 2)
            throw std::runtime_error("bitwise:lshift need at least 2 arguments: bitset, pos");

        if (n[0].valueType() != ValueType::User ||
                !n[0].usertype().is<std::bitset<8>>() )
            throw std::runtime_error("bitwise:lshift the first argument can only be a bitset");

        if (n[1].valueType() != ValueType::Number)
            throw std::runtime_error("bitwise:rshift the second argument can only be a Number");

        std::bitset<8>& bitsetObj = n[0].usertypeRef().as<std::bitset<8>>();
        std::bitset<8>& shifted = bitsetObj << n[1].number();
        return bitset2Ark(shifted);
    }

}

ARK_API_EXPORT mapping *getFunctionsMapping()
{
    mapping *map = mapping_create(3);
    mapping_add(map[0], "bitwise:make-bitset", ArkBitwise::makeBitset);
    mapping_add(map[1], "bitwise:rshift", ArkBitwise::rshift);
    mapping_add(map[2], "bitwise:lshift", ArkBitwise::lshift);
    return map;
}