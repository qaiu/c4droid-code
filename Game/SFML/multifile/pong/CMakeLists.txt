cmake_minimum_required(VERSION 3.7.2)

find_package(SFML 2 REQUIRED COMPONENTS graphics audio)

# all source files
set(SRC Pong.cpp)

# define the pong target
add_executable(pong ${SRC})
target_link_libraries(pong sfml-audio sfml-graphics)
