cmake_minimum_required(VERSION 3.14)
project(my_project)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
	UnitTest_test
	UnitTest.cpp
	Binary_st.cpp
)
add_executable(
	Lab_3
	Lab3.cpp
	Binary_st.cpp
)

target_link_libraries(
	UnitTest_test
	GTest::gtest_main
)
target_link_libraries(
	Lab_3
	pthread
)

include(GoogleTest)
gtest_discover_tests(UnitTest_test)
