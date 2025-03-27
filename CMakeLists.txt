cmake_minimum_required(VERSION 3.17)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json Json)
include_directories(Include)
add_subdirectory(Source)
add_executable(SearchEngine ../Source/main.cpp ../Source/ConverterJSON.cpp ../Include/ConverterJSON.h ../Include/InvertedIndex.h ../Source/InvertedIndex.cpp ../Source/SearchServer.cpp ../Include/SearchServer.h ../Source/EngineExceptions.cpp ../Include/EngineExceptions.h ../Include/tests.h)

target_link_libraries(SearchEngine nlohmann_json::nlohmann_json)

#This part for gtest
#add_subdirectory(googletest-1.16.0)
#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
#set(gtest_disable_pthreads on)
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

#target_link_libraries(SearchEngine gtest gtest_main)
#enable_testing()
