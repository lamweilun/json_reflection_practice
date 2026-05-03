cmake_minimum_required(VERSION 3.31.0)

project(main)
add_executable(main main.cpp)
target_compile_features(main PRIVATE cxx_std_26)
target_compile_options(main PRIVATE -freflection)
