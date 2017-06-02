# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/ffmpeg-config.cpp
             src/main/cpp/native-lib.cpp )




add_library(avcodec-57_lib SHARED IMPORTED)
set_target_properties(avcodec-57_lib PROPERTIES IMPORTED_LOCATION
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi/libavcodec-57.so )

add_library(avdevice-57_lib  SHARED IMPORTED )
set_target_properties(avdevice-57_lib PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi/libavdevice-57.so )

add_library(avfilter-6_lib  SHARED IMPORTED )
set_target_properties(avfilter-6_lib PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi/libavfilter-6.so )

add_library(avformat-57_lib  SHARED IMPORTED )
set_target_properties(avformat-57_lib PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi/libavformat-57.so )

add_library(avutil-55_lib  SHARED IMPORTED )
set_target_properties(avutil-55_lib PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi/libavutil-55.so )

add_library(swresample-2_lib  SHARED IMPORTED )
 set_target_properties(swresample-2_lib PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi/libswresample-2.so )

add_library(swscale-4_lib  SHARED IMPORTED )
set_target_properties(swscale-4_lib PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi/libswscale-4.so )

add_library(avresample-3_lib  SHARED IMPORTED )
set_target_properties(avresample-3_lib PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi/libavresample-3.so )

add_library(x264-148_lib  SHARED IMPORTED )
set_target_properties(x264-148_lib PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi/libx264-148.so )



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

include_directories(src/main/cpp/include/)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       avcodec-57_lib
                       avdevice-57_lib
                       avfilter-6_lib
                       avformat-57_lib
                       avutil-55_lib
                       swresample-2_lib
                       swscale-4_lib
                       avresample-3_lib
                       x264-148_lib


                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )















