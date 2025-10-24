cmake_minimum_required(VERSION 3.10)
project(BetterReadME)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Python3_ROOT_DIR "C:/Users/nicho/AppData/Local/Programs/Python/Python313")

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
add_executable(BetterReadME main.cpp create.cpp changeDir.cpp)

target_include_directories(BetterReadME PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(BetterReadME PRIVATE ${Python3_LIBRARIES})

message(STATUS "Found Python include dir: ${Python3_INCLUDE_DIRS}")
message(STATUS "Found Python library:     ${Python3_LIBRARIES}")
message(STATUS "Found Python executable:  ${Python3_EXECUTABLE}")

