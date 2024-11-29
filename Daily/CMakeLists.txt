cmake_minimum_required(VERSION 3.30)
project(daily C)

set(CMAKE_C_STANDARD 11)
include_directories("C:\\Sqlite")

add_executable(daily daily.c)

SET(CMAKE_EXE_LINKER_FLAGS "-lsqlite3 -LC:\\Sqlite")
SET(CMAKE_SHARED_LINKER_FLAGS "-lsqlite3 -LC:\\Sqlite" )
