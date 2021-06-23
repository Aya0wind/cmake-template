#pragma once
#include <format>
#include <iostream>
#include <ostream>
#include <ranges>
#include <type_traits>

#include "leetcode_types.hpp"

namespace tools {
template <class T>
concept Format = requires(T t) {
    { std::format("{}", t) } -> std::convertible_to<std::string>;
};

template <class T>
concept Container = std::ranges::input_range<std::remove_cvref_t<T>>;

template <class T>
concept IsStdString = std::same_as<std::string, std::remove_cvref_t<T>>;

template <class T>
concept Printable = Container<T> || Format<T>;

[[nodiscard, maybe_unused]] inline std::string to_string(const LinkedList& list) {
    std::string ss;
    ListNode* curNode = list.head;
    while (curNode) {
        ss += std::to_string(curNode->val);
        ss += " -> ";
        curNode = curNode->next;
    }
    ss += "null";
    return ss;
}
[[nodiscard, maybe_unused]] inline std::string to_string(const BinaryTree& tree) {
    std::vector<std::optional<int>> flatIntList = flattenBinTree(tree);
    std::vector<std::string> flatStrList = convertStrList(flatIntList);
    std::string flatStr;
    for (auto&& str : flatStrList) {
        flatStr += str;
        flatStr += ',';
    }
    return flatStr;
}

template <Printable... Types>
[[maybe_unused]] void println(const std::string_view fmtStr, const Types&... args) {
    std::cerr << std::format(fmtStr, args...) << "\n";
}

template <Printable... Types>
[[maybe_unused]] void println(std::ostream& out, const std::string_view fmtStr, const Types&... args) {
    out << std::format(fmtStr, args...) << "\n";
}

}  // namespace tools

namespace std {

template <class CharT>
struct [[maybe_unused]] formatter<tools::LinkedList, CharT> {
    [[nodiscard, maybe_unused]] auto parse(format_parse_context& ctx) {
        return ctx.end();
    }
    template <typename OutputIt>
    [[nodiscard, maybe_unused]] auto
    format(const tools::LinkedList& value, std::basic_format_context<OutputIt, char>& ctx) const noexcept {
        auto output = ctx.out();
        auto valueString = tools::to_string(value);
        output = std::copy(valueString.begin(), valueString.end(), output);
        return output;
    }
};

template <class CharT>
struct [[maybe_unused]] formatter<tools::BinaryTree, CharT> {
    [[nodiscard, maybe_unused]] auto parse(format_parse_context& ctx) {
        return ctx.end();
    }
    template <typename OutputIt>
    [[nodiscard, maybe_unused]] auto
    format(const tools::BinaryTree& value, std::basic_format_context<OutputIt, char>& ctx) const noexcept {
        auto output = ctx.out();
        auto valueString = tools::to_string(value);
        output = std::copy(valueString.begin(), valueString.end(), output);
        return output;
    }
};

template <tools::Container T, class CharT>
struct [[maybe_unused]] formatter<T, CharT> {
    [[nodiscard, maybe_unused]] auto parse(format_parse_context& ctx) {
        return ctx.end();
    }
    template <typename OutputIt>
    [[nodiscard, maybe_unused]] auto
    format(const T& value, std::basic_format_context<OutputIt, char>& ctx) const noexcept {
        if constexpr (tools::IsStdString<T>) {
            return std::formatter<std::string, CharT>::format(value);
        }
        auto output = ctx.out();
        bool first = true;
        *output++ = '[';
        for (auto&& e : value) {
            if (first) {
                first = false;
                auto valueString = std::format("{}", e);
                output = std::copy(valueString.begin(), valueString.end(), output);
            } else {
                *output++ = ',';
                auto valueString = std::format("{}", e);
                output = std::copy(valueString.begin(), valueString.end(), output);
            }
        }
        *output = ']';
        return output;
    }
};

template <tools::Printable... T, class CharT>
struct [[maybe_unused]] formatter<std::tuple<T...>, CharT> {
    [[maybe_unused]] [[nodiscard, maybe_unused]] auto parse(format_parse_context& ctx) {
        return ctx.end();
    }
    template <typename OutputIt>
    [[nodiscard, maybe_unused]] auto
    format(const std::tuple<T...>& value, std::basic_format_context<OutputIt, char>& ctx) const noexcept {
        auto output = ctx.out();
        *output++ = '(';
        bool first = true;
        auto format_tuple = [ &first, &output ](const auto& value) {
            if (first) {
                first = false;
                auto valueString = std::format("{}", value);
                output = std::copy(valueString.begin(), valueString.end(), output);
            } else {
                *output++ = ',';
                *output++ = ' ';
                auto valueString = std::format("{}", value);
                output = std::copy(valueString.begin(), valueString.end(), output);
            }
        };
        std::apply([ format_tuple ](auto&&... args) { ((format_tuple(args)), ...); }, value);
        *output++ = ')';
        return output;
    }
};

template <tools::Printable T1, tools::Printable T2, class CharT>
struct [[maybe_unused]] formatter<std::pair<T1, T2>, CharT> {
    [[maybe_unused]] [[nodiscard, maybe_unused]] auto parse(format_parse_context& ctx) {
        return ctx.end();
    }
    template <typename OutputIt>
    [[nodiscard, maybe_unused]] auto
    format(const std::pair<T1, T2>& value, std::basic_format_context<OutputIt, char>& ctx) const noexcept {
        auto output = ctx.out();
        *output++ = '(';
        auto first = std::format("{}", value.first);
        output = std::copy(first.begin(), first.end(), output);
        *output++ = ' ';
        *output++ = ':';
        *output++ = ' ';
        auto second = std::format("{}", value.first);
        output = std::copy(second.begin(), second.end(), output);
        *output++ = ')';
        return output;
    }
};
}  // namespace std
