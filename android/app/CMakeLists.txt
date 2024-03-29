# Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.

cmake_minimum_required(VERSION 3.4.1)

# Specifies a library name, specifies whether the library is STATIC or
# SHARED, and provides relative paths to the source code. You can
# define multiple libraries by adding multiple add_library() commands,
# and CMake builds them for you. When you build your app, Gradle
# automatically packages shared libraries with your APK.

file(GLOB_RECURSE srcs "../../src/*.cpp")

link_directories( build/ndkLibs/${ANDROID_ABI}/ )

add_library( # Specifies the name of the library.
             ruisapp_application # this has to be 'ruisapp_application' so that RuisappActivity loads that named .so

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${srcs} )



include_directories( build/ndkLibs/include/ )

target_link_libraries(
        ruisapp_application
        ruisapp ruis-render-opengles ruis log GLESv2 stob papki svgren svgdom unikod png z jpeg freetype2 EGL android nitki mikroxml cairo pixman
    )
