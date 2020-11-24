#include <picosha2.hpp>

#include <Ark/Module.hpp>

namespace ArkHash
{
    Value sha2(std::vector<Value>& n, Ark::VM* vm)
    {
        if (n.size() != 1)
            throw std::runtime_error("hash:sha256 need a single argument: data");
        if (n[0].valueType() != ValueType::String)
            throw std::runtime_error("hash:sha256: data must be a String");

        std::string src = n[0].string_ref().toString();
        std::string output = "";
        picosha2::hash256_hex_string(src, output);

        return Value(output);
    }
}

ARK_API_EXPORT Mapping_t getFunctionsMapping()
{
    Mapping_t map;
    map["hash:sha256"] = ArkHash::sha2;

    return map;
}