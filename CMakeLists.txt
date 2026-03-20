cmake_minimum_required(VERSION 3.31)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(svr)
#project(ipUtil)

find_package(Crow)
find_package(OpenSSL REQUIRED)

add_executable(${PROJECT_NAME}
src/ip_utils.c
src/read_write_handler.c
src/checksum_handler.c
src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(${PROJECT_NAME} PUBLIC Crow::Crow)
