#include <string>
#include <unordered_map>
#include <random>
#include <chrono>

#include <Ark/Module.hpp>

namespace ArkRandom
{
    Value random(std::vector<Value>& n [[maybe_unused]], Ark::VM* vm [[maybe_unused]])
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 g(seed);
        int x = static_cast<int>(g());
        x = (x > 0) ? x : -x;
        x %= 32768;
        return Value(x);
    }
}

ARK_API mapping* getFunctionsMapping()
{
    mapping* map = mapping_create(1);
    mapping_add(map[0], "random", ArkRandom::random);

    return map;
}
