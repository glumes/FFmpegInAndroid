# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(lib_src_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})

include_directories(
        ${CMAKE_SOURCE_DIR}/src/main/cpp/include
)



add_library(avcodec-57_lib SHARED IMPORTED)
set_target_properties(avcodec-57_lib PROPERTIES IMPORTED_LOCATION
                             ${lib_src_DIR}/libavcodec-57.so)


add_library(avformat-57_lib SHARED IMPORTED)
set_target_properties(avformat-57_lib PROPERTIES IMPORTED_LOCATION
                        ${lib_src_DIR}/libavformat-57.so)

add_library(avutil-55_lib SHARED IMPORTED)
set_target_properties(avutil-55_lib PROPERTIES IMPORTED_LOCATION
                        ${lib_src_DIR}/libavutil-55.so)

add_library(swresample-2_lib SHARED IMPORTED)
set_target_properties(swresample-2_lib PROPERTIES IMPORTED_LOCATION
                        ${lib_src_DIR}/libswresample-2.so)

add_library(swscale-4_lib SHARED IMPORTED)
set_target_properties(swscale-4_lib PROPERTIES IMPORTED_LOCATION
                        ${lib_src_DIR}/libswscale-4.so)


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries(
log
avcodec-57_lib
avformat-57_lib
avutil-55_lib
swresample-2_lib
swscale-4_lib)