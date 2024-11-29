cmake_minimum_required(VERSION 3.30)
project(weekly C)

set(CMAKE_C_STANDARD 11)
include_directories("C:\\Sqlite")

add_executable(weekly weekly.c)

SET(CMAKE_EXE_LINKER_FLAGS "-lsqlite3 -LC:\\Sqlite")
SET(CMAKE_SHARED_LINKER_FLAGS "-lsqlite3 -LC:\\Sqlite" )