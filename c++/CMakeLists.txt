cmake_minimum_required(VERSION 3.0.0)
project(cpp VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(cpp main.cpp)

target_sources(cpp
    PRIVATE
    c++.cpp
    class.cpp
    const.cpp
    exception.cpp
    fundamental_type.cpp
    hash.cpp
    inheritance.cpp
    inline.cpp
    lambda.cpp
    macro.cpp
    main.cpp
    return_value_optimization.cpp
    singleton.cpp
    sizeof.cpp
    static.cpp
    stl.cpp
    stl_algorithm.cpp
    stl_array.cpp
    stl_chrono.cpp
    stl_forward_list.cpp
    stl_iterator.cpp
    stl_list.cpp
    stl_map.cpp
    stl_reverse_iterator.cpp
    stl_set.cpp
    stl_string.cpp
    stl_stringstream.cpp
    stl_tuple.cpp
    stl_unordered_set.cpp
    stl_vector.cpp
    switch.cpp
    template.cpp
    undefined_behavior.cpp
    structured_binding.cpp
    stl_regex.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
