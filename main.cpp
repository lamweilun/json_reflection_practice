#include "json_parser.hpp"

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

#include <print>

struct Foo
{
    int                                 i                 = 123;
    float                               f                 = 3.1415f;
    bool                                b                 = false;
    std::string_view                    str               = "Foo";
    std::string_view                    str_view          = "Foo string_view";
    std::pair<int, std::string>         pair              = {456, "pair"};
    std::tuple<int, float, std::string> tup               = {789, -1.9f, "tuple"};
    int                                 c_array[5]        = {-1, -2, -3, -4, -5};
    std::array<int, 5>                  std_array         = {1, 2, 3, 4, 5};
    std::vector<float>                  std_vector        = {1.23f, 4.56f, 7.89f};
    std::set<std::string>               std_set           = {"Hello", "World"};
    std::unordered_set<std::string>     std_unordered_set = {"Hello", "World"};
    std::map<std::string, int>          std_map           = {
        {"Hello", 1},
        {"World", 2}
    };
    std::unordered_map<std::string_view, bool> std_unordered_map = {
        {"Hello",  true},
        {"World", false}
    };

    enum Color
    {
        Red,
        Blue,
        White
    };
    Color color;
};

struct Bar : Foo
{
    std::optional<Color> optional_color  = Color::Red;
    std::optional<Color> optional_color2 = std::nullopt;

    std::variant<int, float, bool> default_variant;
    std::variant<int, std::string> int_or_string1 = 1;
    std::variant<int, std::string> int_or_string2 = "std::string";

    std::unique_ptr<std::string> string_unique_ptr_null = nullptr;
    std::unique_ptr<std::string> string_unique_ptr      = std::make_unique<std::string>("std::unique_ptr");
};

struct Baz
{
    Bar bar;
};

int main()
{
    std::println("{}", parse_json(Baz{}));
}
