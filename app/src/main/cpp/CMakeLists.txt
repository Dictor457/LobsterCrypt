cmake_minimum_required(VERSION 3.22.1)
project("lobstercrypt")

add_library(lobster_crypt SHARED crypt_shell.cpp)

find_library(log-lib log)

target_link_libraries(lobster_crypt ${log-lib})
