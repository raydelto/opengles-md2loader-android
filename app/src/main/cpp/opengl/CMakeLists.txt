cmake_minimum_required(VERSION 3.4.1)

add_library( # Sets the name of the library.
        MD2Loader

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        Renderer.cpp
        Renderer.h
        MD2Model.cpp
        native-lib.cpp
        ShaderProgram.cpp
        Texture2D.cpp)

include_directories(
        ../third_party_libraries)

find_library(log-lib
        log)

target_link_libraries(
        MD2Loader
        EGL
        GLESv2
        android
        ${log-lib})



