cmake_minimum_required(VERSION 3.31)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
project(svr)

find_package(OpenSSL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MHD REQUIRED libmicrohttpd)

add_executable(${PROJECT_NAME}
src/main.c
src/read_write_handler.c
src/checksum_handler.c
src/ip_utils.c
src/hashmap.c
src/mime.c
src/url.c
)

target_include_directories(${PROJECT_NAME} PRIVATE include ${MHD_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto ${MHD_LIBRARIES})

