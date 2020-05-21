#include <string>
#include <unordered_map>
#include <random>
#include <chrono>

#include <Ark/Module.hpp>

namespace ArkRandom
{
    Value random(std::vector<Value>& n, Ark::VM* vm)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 g(seed);
        int x = static_cast<int>(g());
        x = (x > 0) ? x : -x;
        x %= 16384;
        return Value(x);
    }

    Value random_10(std::vector<Value>& n, Ark::VM* vm)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 g(seed);
        int x = static_cast<int>(g());
        x = (x > 0) ? x : -x;
        x %= 10;
        return Value(x);
    }
}

ARK_API_EXPORT Mapping_t getFunctionsMapping()
{
    Mapping_t map;
    map["random"] = ArkRandom::random;
    map["rand10"] = ArkRandom::random_10;

    return map;
}