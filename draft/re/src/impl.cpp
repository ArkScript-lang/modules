#include <Ark/VM/Value/Dict.hpp>
#include <module.hpp>
#include <utils.hpp>

#include <vector>
#include <ranges>

#include <re2/re2.h>

namespace Regex
{
    using namespace Ark;
    using namespace Ark::literals;
    using namespace re2;

    Value toValue(const Match& match, const std::string& text)
    {
        internal::Dict dict;
        const auto& span = match.spans.front();
        if (!span.empty)
            dict.set(Value("match"), Value(text.substr(span.start, span.size)));
        else
            dict.set(Value("match"), Nil);
        dict.set(Value("start"), Value(static_cast<int64_t>(span.start)));
        dict.set(Value("end"), Value(static_cast<int64_t>(span.size + span.start)));

        Value groups(ValueType::List);
        for (const Span& s : match.spans | std::ranges::views::drop(1))
            groups.push_back(Value(text.substr(s.start, s.size)));
        dict.set(Value("groups"), groups);

        return Value(std::move(dict));
    }

    Value toValue(const MatchImpl_t& res)
    {
        if (res.empty)
            return Nil;

        if (res.expect_many)
        {
            Value output(ValueType::List);
            for (const Match& match : res.matches)
            {
                // todo: create a dict with all the groups
                output.push_back(toValue(match, res.text));
            }
            return output;
        }

        const Match& match = res.matches.front();
        return toValue(match, res.text);
    }

    Value search(std::vector<Value>& args, VM* vm)
    {
        if (!types::check(args, ValueType::String, ValueType::String))
            throw types::TypeCheckingError(
                "re:search",
                { { types::Contract { { types::Typedef("regex", ValueType::String), types::Typedef("input", ValueType::String) } } } },
                args);
        const std::string& regex = args[0].string();
        const std::string& input = args[1].string();
        return toValue(matchImpl(RE2::UNANCHORED, regex, input, true));
    }

    Value match(std::vector<Value>& args, VM* vm)
    {
        if (!types::check(args, ValueType::String, ValueType::String))
            throw types::TypeCheckingError(
                "re:match",
                { { types::Contract { { types::Typedef("regex", ValueType::String), types::Typedef("input", ValueType::String) } } } },
                args);
        const std::string& regex = args[0].string();
        const std::string& input = args[1].string();
        return toValue(matchImpl(RE2::ANCHOR_START, regex, input, true));
    }

    Value full_match(std::vector<Value>& args, VM* vm)
    {
        if (!types::check(args, ValueType::String, ValueType::String))
            throw types::TypeCheckingError(
                "re:fullMatch",
                { { types::Contract { { types::Typedef("regex", ValueType::String), types::Typedef("input", ValueType::String) } } } },
                args);
        const std::string& regex = args[0].string();
        const std::string& input = args[1].string();
        return toValue(matchImpl(RE2::ANCHOR_BOTH, regex, input, true));
    }

    Value find_all(std::vector<Value>& args, VM* vm)
    {
        if (!types::check(args, ValueType::String, ValueType::String))
            throw types::TypeCheckingError(
                "re:findAll",
                { { types::Contract { { types::Typedef("regex", ValueType::String), types::Typedef("input", ValueType::String) } } } },
                args);
        const std::string& regex = args[0].string();
        const std::string& input = args[1].string();
        return toValue(matchImpl(RE2::UNANCHORED, regex, input, false));
    }

    Value sub(std::vector<Value>& args, VM* vm)
    {
        if (!types::check(args, ValueType::String, ValueType::String, ValueType::String))
            throw types::TypeCheckingError(
                "re:sub",
                { { types::Contract { { types::Typedef("regex", ValueType::String),
                                        types::Typedef("replacement", ValueType::String),
                                        types::Typedef("input", ValueType::String) } } } },
                args);
        const std::string& regex = args[0].string();
        const std::string& replacement = args[1].string();
        std::string input = args[2].string();

        // todo: control how many times we want to replace?
        RE2::GlobalReplace(&input, regex, replacement);
        return Value(input);
    }

    Value split(std::vector<Value>& args, VM* vm)
    {
        if (!types::check(args, ValueType::String, ValueType::String) && !types::check(args, ValueType::String, ValueType::String, ValueType::Number))
            throw types::TypeCheckingError(
                "re:split",
                { { types::Contract {
                        { types::Typedef("regex", ValueType::String),
                          types::Typedef("input", ValueType::String) } },
                    types::Contract { { types::Typedef("regex", ValueType::String),
                                        types::Typedef("input", ValueType::String),
                                        types::Typedef("max_split", ValueType::Number) } } } },
                args);
        const std::string& regex = args[0].string();
        const std::string& input = args[1].string();
        long max_split = 0;
        if (args.size() == 3)
            max_split = static_cast<long>(args[2].number());

        Value output(ValueType::List);
        if (max_split < 0)
        {
            output.push_back(args[1]);
            return output;
        }

        long num_split = 0;
        std::size_t end = 0;
        const MatchImpl_t res = matchImpl(RE2::UNANCHORED, regex, input, false);
        if (res.empty)
            return Nil;

        for (const Match& match : res.matches)
        {
            output.push_back(Value(res.text.substr(end, match.start() - end)));
            for (std::size_t i = 1; i < match.spans.size(); ++i)
            {
                if (!match.spans[i].empty)
                    output.push_back(Value(res.text.substr(match.spans[i].start, match.spans[i].size)));
                else
                    output.push_back(Nil);
            }

            end = match.end();
            num_split++;

            if (num_split == max_split)
                break;
        }
        if (end < res.text.size())
            output.push_back(Value(res.text.substr(end)));

        return output;
    }

    Value escape(std::vector<Value>& args, VM* vm)
    {
        if (!types::check(args, ValueType::String))
            throw types::TypeCheckingError(
                "re:escape",
                { { types::Contract { { types::Typedef("text", ValueType::String) } } } },
                args);
        return Value(RE2::QuoteMeta(args[0].string()));
    }
}
