cmake_minimum_required(VERSION 3.29)
project(FinalProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK4 REQUIRED gtk4)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})


include_directories("/opt/homebrew/Cellar/json-c/0.18/include")
link_directories("/opt/homebrew/Cellar/json-c/0.18/include")


add_definitions(${GTK4_CFLAGS_OTHER})
add_definitions(-DROOT_PATH="${CMAKE_BINARY_DIR}")

add_executable(FinalProject main.c
        Screens/MainScreen/MainScreen.h
        Screens/MainScreen/MainScreen.c
        sqlite/daily/daily.h
        sqlite/daily/daily.c
        sqlite/weekly/weekly.h
        sqlite/weekly/weekly.c
)

target_link_libraries(FinalProject ${GTK4_LIBRARIES} sqlite3)

add_custom_target(copy_ui_file ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/src
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src ${CMAKE_BINARY_DIR}/src
)

add_dependencies(FinalProject copy_ui_file)