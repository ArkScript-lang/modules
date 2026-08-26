#include <module.hpp>

ARK_API Ark::mapping* getFunctionsMapping()
{
    static Ark::mapping map[] = {
        { "re:search", Regex::search },
        { "re:match", Regex::match },
        { "re:fullMatch", Regex::full_match },
        { "re:findAll", Regex::find_all },
        { "re:sub", Regex::sub },
        { "re:split", Regex::split },
        { "re:escape", Regex::escape },
        { nullptr, nullptr }
    };

    return map;
}
