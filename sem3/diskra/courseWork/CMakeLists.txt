cmake_minimum_required(VERSION 3.10)
project(FiniteCalculator VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(core/include)

# фикс ворнинга pybind11
cmake_policy(SET CMP0148 NEW)
find_package(Python 3 COMPONENTS Interpreter Development)

find_package(pybind11 REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(GTest REQUIRED) 

set(CORE_SOURCES
    core/src/RingNumber.cc
    core/src/FiniteRingRules.cc
    core/src/SmallRingArithmetic.cc 
    core/src/BigRingArithmetic.cc
        core/src/utils.cc
)

# модуль для python
pybind11_add_module(finite_ring_module
    ${CORE_SOURCES}
    core/py_binding.cc
)
target_link_libraries(finite_ring_module PRIVATE yaml-cpp::yaml-cpp)


# --- GTest / CTest ИНТЕГРАЦИЯ ---

enable_testing()

# тест 1: малая арифметика
add_executable(test_small
    core/src/FiniteRingRules.cc
    core/src/SmallRingArithmetic.cc
    tests/test_small_arithmetic.cc
)
target_link_libraries(test_small PRIVATE yaml-cpp::yaml-cpp GTest::gtest_main)

# тест 2: RingNumber
add_executable(test_number
    core/src/FiniteRingRules.cc
    core/src/RingNumber.cc
    tests/test_ring_number.cc
)
target_link_libraries(test_number PRIVATE yaml-cpp::yaml-cpp GTest::gtest_main)

# тест 3: большая арифметика
add_executable(test_big
    ${CORE_SOURCES}
    tests/test_big_arithmetic.cc
)
target_link_libraries(test_big PRIVATE yaml-cpp::yaml-cpp GTest::gtest_main)

# * регистрация тестов
gtest_discover_tests(test_small)
gtest_discover_tests(test_number)
gtest_discover_tests(test_big)

# enable_testing()
# add_test(NAME Test_Z8_Variant_1 COMMAND test_runner variant_1)
# add_test(NAME Test_Z8_Variant_2 COMMAND test_runner variant_2)
# # add_test(NAME Test_Z8_X1 COMMAND test_runner X1)
# add_test(NAME Test_Z11_D1 COMMAND test_runner D1)
