#ifndef RE_UTILS_HPP
#define RE_UTILS_HPP

#include <vector>
#include <string>

#include <re2/re2.h>
#include <absl/strings/string_view.h>

namespace Regex
{
    struct Span
    {
        long start;
        std::size_t size;
        bool empty;
    };

    struct Match
    {
        std::vector<Span> spans;

        [[nodiscard]] inline long start() const
        {
            return spans.front().start;
        }

        [[nodiscard]] inline std::size_t end() const
        {
            return spans.front().start + spans.front().size;
        }

        [[nodiscard]] inline std::size_t size() const
        {
            return spans.front().size;
        }
    };

    struct MatchImpl_t
    {
        std::string text;
        std::vector<Match> matches;
        bool empty;
        bool expect_many;
    };

    inline Span asSpan(const absl::string_view& sv, const std::string& text)
    {
        return Span { .start = static_cast<long>(sv.data() - text.data()), .size = sv.size(), .empty = false };
    }

    inline Span getFirstSpan(const std::vector<absl::string_view>& groups, const std::string& text)
    {
        if (const auto& it = groups.front(); it.data() == nullptr)
            return Span { .start = 0, .size = 0, .empty = true };
        else
            return asSpan(it, text);
    }

    MatchImpl_t matchImpl(RE2::Anchor anchor, const std::string& regex, const std::string& text, bool stop_at_first);
}

#endif  // RE_UTILS_HPP
