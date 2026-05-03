# C++26 Struct To JSON Parser using C++ Reflection

## Requirements
- Only compiled and tested on GCC 16.1 with `-freflection` flag.

## Features
- Automatic json serialization from a C++ struct/class.
- Supports enum to string.
- Supports inheritance.
- Supports some STL libraries, namely...
    - `std::string/string_view`
    - `std::array`
    - `std::vector`
    - `std::set/unordered_set`
    - `std::map/unordered_map`
    - `std::unique_ptr`
    - `std::pair/tuple`
    - `std::optional`
    - `std::variant`


## Remarks
- This parser is written as a form of practice.
- Please take a look at [glaze](github.com/stephenberry/glaze) for a proper C++26 reflection based JSON serialization library.
