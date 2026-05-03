#pragma once

#include <meta>  // reflection header

#include <format>

#include <array>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

template <typename T>
constexpr auto parse_json(T const& t)
{
    std::string    json;
    constexpr auto reflected_type = ^^T;

    // Helper to output delimiter and toggle first flag
    auto delim = [&json, first = true]() mutable
    {
        if (!first)
        {
            json += ", ";
        }
        first = false;
    };

    if constexpr (is_enum_type(reflected_type))
    {
        template for (constexpr auto e :
                      define_static_array(enumerators_of(reflected_type)))
        {
            if (t == [:e:])
            {
                json += std::format("\"{}\"", identifier_of(e));
            }
        }
    }
    else if constexpr (is_integral_type(reflected_type) ||
                       is_floating_point_type(reflected_type))
    {
        json += std::format("{}", t);
    }
    else if constexpr (is_same_type(reflected_type, ^^std::string) ||
                       is_same_type(reflected_type, ^^std::string_view))
    {
        json += std::format("\"{}\"", t);
    }
    else if constexpr (has_template_arguments(reflected_type) &&
                       template_of(reflected_type) == ^^std::optional)
    {
        json += t.has_value() ? parse_json(t.value()) : "null";
    }
    else if constexpr (is_pointer_type(reflected_type))
    {
        // Does not support raw pointer
    }
    else if constexpr (has_template_arguments(reflected_type) &&
                       (template_of(reflected_type) == ^^std::shared_ptr ||
                        template_of(reflected_type) == ^^std::weak_ptr))
    {
        // Does not support shared_ptr/weak_ptr
    }
    else if constexpr (has_template_arguments(reflected_type) &&
                       template_of(reflected_type) == ^^std::unique_ptr)
    {
        json += t == nullptr ? "null" : parse_json(*t);
    }
    else if constexpr (has_template_arguments(reflected_type) &&
                       template_of(reflected_type) == ^^std::variant)
    {
        t.visit([&json](auto const& v)
                {
                    json += parse_json(v);
                });
    }
    else if constexpr (has_template_arguments(reflected_type) &&
                       (template_of(reflected_type) == ^^std::pair ||
                        template_of(reflected_type) == ^^std::tuple))
    {
        json += '[';
        template for (auto const& elem : t)
        {
            delim();
            json += parse_json(elem);
        }
        json += ']';
    }
    else if constexpr (is_array_type(reflected_type) ||
                       (has_template_arguments(reflected_type) &&
                        (template_of(reflected_type) == ^^std::array ||
                         template_of(reflected_type) == ^^std::vector ||
                         template_of(reflected_type) == ^^std::set ||
                         template_of(reflected_type) == ^^std::unordered_set)))
    {
        json += '[';
        for (auto const& elem : t)
        {
            delim();
            json += parse_json(elem);
        }
        json += ']';
    }
    else if constexpr (has_template_arguments(reflected_type) &&
                       (template_of(reflected_type) == ^^std::map ||
                        template_of(reflected_type) == ^^std::unordered_map))
    {
        json += '{';
        for (auto const& [key, data] : t)
        {
            delim();
            json += std::format("{}: {}", parse_json(key), parse_json(data));
        }
        json += '}';
    }
    else if constexpr (is_object_type(reflected_type))
    {
        json += '{';
        constexpr auto ctx = std::meta::access_context::current();

        template for (constexpr auto base :
                      define_static_array(bases_of(^^T, ctx)))
        {
            delim();
            json += std::format("\"{}\": {}", identifier_of(base),
                                parse_json((typename[:type_of(base):] const&)(t)));
        }
        template for (constexpr auto mem :
                      define_static_array(nonstatic_data_members_of(^^T, ctx)))
        {
            delim();
            std::string_view mem_label =
                has_identifier(mem) ? identifier_of(mem) : "(unnamed-member)";
            json += std::format("\"{}\": {}", mem_label, parse_json(t.[:mem:]));
        }
        json += '}';
    }

    return json;
}
