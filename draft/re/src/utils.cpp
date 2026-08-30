#include <utils.hpp>

#include <fmt/format.h>

namespace Regex
{
    MatchImpl_t matchImpl(const RE2::Anchor anchor, const std::string& regex, const std::string& text, const bool stop_at_first)
    {
        constexpr std::size_t start_pos = 0;
        const std::size_t end_pos = text.size();

        auto options = RE2::Options(RE2::Quiet);
        options.set_case_sensitive(true);
        options.set_posix_syntax(false);
        options.set_perl_classes(true);

        RE2 pattern(regex, options);
        if (!pattern.ok())
            throw std::runtime_error(fmt::format("re: error while compiling pattern '{}', {}", regex, pattern.error()));

        const int num_groups = 1 + pattern.NumberOfCapturingGroups();  // +1 to have $0 as well
        std::vector<absl::string_view> groups;
        groups.resize(num_groups);

        if (!pattern.Match(text, start_pos, end_pos, anchor, groups.data(), num_groups))
            return { .empty = true };

        MatchImpl_t output {
            .text = text,
            .empty = false,
            .expect_many = !stop_at_first
        };
        if (stop_at_first)
        {
            Match match;
            for (const auto& it : groups)
            {
                if (it.data() == nullptr)
                    match.spans.emplace_back(Span { 0, 0, true });
                else
                    match.spans.emplace_back(asSpan(it, text));
            }
            output.matches.emplace_back(match);
        }
        else
        {
            std::size_t pos = start_pos;
            Span span = getFirstSpan(groups, text);

            while (true)
            {
                if (span.empty)
                    break;

                std::vector<Span> spans;
                for (const auto& it : groups)
                {
                    if (it.data() == nullptr)
                        spans.emplace_back(Span { 0, 0, true });
                    else
                        spans.emplace_back(asSpan(it, text));
                }
                output.matches.emplace_back(Match { .spans = spans });

                if (pos == end_pos)
                    break;
                if (pos == span.start + span.size)
                    // we matched the empty string at `pos` and would be stuck, so in order to make progress,
                    // increment the offset
                    pos++;
                else
                    pos = span.start + span.size;

                // todo: should we reset `groups` each time?
                if (!pattern.Match(text, pos, end_pos, anchor, groups.data(), num_groups))
                    break;
                span = getFirstSpan(groups, text);
            }
        }
        return output;
    }
}
