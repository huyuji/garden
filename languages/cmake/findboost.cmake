cmake_minimum_required(VERSION 2.8.7)

project(test_cmake)

set (BOOST_INCLUDEDIR  "e:/program/boost/boost_1_58_0")
set (BOOST_LIBRARYDIR "e:/program/boost/boost_1_58_0/lib32-msvc-10.0")
set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.58.0 COMPONENTS date_time filesystem system)
if(Boost_FOUND)
          message(STATUS "${Boost_INCLUDE_DIRS}")
          message(STATUS "${Boost_LIBRARIES}")
endif()