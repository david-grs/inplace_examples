cmake_minimum_required(VERSION 2.8)

project(inplace_examples)
add_executable(inplace_examples inplace_examples.cpp)

if (MSVC)
	set(cxx_compile_options /std:c++17 /W4 /WX)

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
else()
	set(cxx_compile_options -std=c++14 -g -Wall -Wextra -Wpedantic -Wconversion -Wswitch-default -Wswitch-enum -Wunreachable-code -Wwrite-strings -Wcast-align -Wshadow -Wundef -Wno-switch-default -Wold-style-cast -Wshadow -Wdouble-promotion)

	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(cxx_compile_options ${cxx_compile_options} -Weverything -Wno-c++98-compat -Wno-global-constructors)
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		set(cxx_compile_options ${cxx_compile_options} -Wuseless-cast -Wduplicated-cond -Wlogical-op)
	endif()

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

	if (COVERAGE)
		target_compile_options(tests PRIVATE --coverage)
		target_link_libraries(tests PRIVATE --coverage)
	endif()
endif()

target_compile_options(inplace_examples PRIVATE ${cxx_compile_options})

