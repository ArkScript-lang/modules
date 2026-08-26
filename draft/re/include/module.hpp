#ifndef RE_REGEX_HPP
#define RE_REGEX_HPP

#include <Ark/Module.hpp>

namespace Regex
{
    using namespace Ark;

    Value search(std::vector<Value>& args, VM* vm);
    Value match(std::vector<Value>& args, VM* vm);
    Value full_match(std::vector<Value>& args, VM* vm);
    Value find_all(std::vector<Value>& args, VM* vm);
    Value sub(std::vector<Value>& args, VM* vm);
    Value split(std::vector<Value>& args, VM* vm);
    Value escape(std::vector<Value>& args, VM* vm);
}

#endif // RE_REGEX_HPP
