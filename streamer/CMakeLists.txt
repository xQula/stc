cmake_minimum_required(VERSION 3.5)

project(streamer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(streamer main.cpp SyncStream.cpp)

target_link_libraries(streamer PUBLIC pthread)
