cmake_minimum_required(VERSION 3.4.1)

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        Renderer.cpp
        Renderer.h
        Md2.cpp
        native-lib.cpp
        ShaderProgram.cpp
        Texture2D.cpp)

find_library(log-lib
                log)

target_link_libraries(
        native-lib
        EGL
        GLESv2
        android
        ${log-lib})



