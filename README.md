# C-program
CMakeList.txt
```aiignore
add_definitions(-DROOT_PATH="${CMAKE_BINARY_DIR}")
```
```aiignore
add_custom_target(copy_ui_file ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/src
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src ${CMAKE_BINARY_DIR}/src
)
```

```aiignore
add_executable(FinalProject MACOSX_BUNDLE main.c
        Screens/MainScreen/MainScreen.c
        Screens/MainScreen/MainScreen.h
        Screens/MainScreen/MainScreenFunctions.h
        Screens/MainScreen/MainScreenFunctions.c)
```
