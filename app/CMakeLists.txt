cmake_minimum_required(VERSION 3.22.1)
project("lobster_crypt")

add_library(lobster_crypt SHARED src/main/cpp/crypt_shell.cpp)

find_library(log-lib log)
target_link_libraries(lobster_crypt ${log-lib})
