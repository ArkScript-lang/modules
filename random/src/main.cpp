#include <string>
#include <unordered_map>
#include <random>
#include <chrono>

#include <Ark/Module.hpp>

namespace ArkRandom
{
    using namespace Ark;

    Value random(std::vector<Value>& n [[maybe_unused]], VM* vm [[maybe_unused]])
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 g(seed);
        int x = static_cast<int>(g());
        x = (x > 0) ? x : -x;
        x %= 32768;
        return Value(x);
    }
}

ARK_API Ark::mapping* getFunctionsMapping()
{
    static Ark::mapping map[] = {
        { "random", ArkRandom::random },
        { nullptr, nullptr }
    };

    return map;
}
