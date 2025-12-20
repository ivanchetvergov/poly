cmake_minimum_required(VERSION 3.10)
project(Multiset)

include_directories(include)

add_executable(multiset_app
    src/main.cc
    src/Core.cc
    src/IO.cc
    src/Multiset_Fill.cc
    src/Multiset_Helpers.cc
    src/Multiset_Operators.cc
    src/UIHandler.cc
    src/MultisetArithmetic.cc
)

set_property(TARGET multiset_app PROPERTY CXX_STANDARD 17)
set_property(TARGET multiset_app PROPERTY CXX_STANDARD_REQUIRED ON)