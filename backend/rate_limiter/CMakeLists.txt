cmake_minimum_required(VERSION 3.5)
project(rate_limiter CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Drogon REQUIRED)

add_executable(rate_limiter main.cc)
target_link_libraries(rate_limiter PRIVATE Drogon::Drogon)