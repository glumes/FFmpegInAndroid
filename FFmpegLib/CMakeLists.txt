# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)


add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/ffmpeg-config.cpp
             src/main/cpp/native-lib.cpp )

add_library( # Sets the name of the library.
             media-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/include/mediadata.cpp
             )


add_library( # Sets the name of the library.
             simple-mediadata-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/include/libsimple_mediadata/simple_mediadata.cpp
             )


add_library( # Sets the name of the library.
             ffmpeg-examples-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/include/ffmpeg_examples/decode_audio.cpp
             )

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




find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

include_directories(src/main/cpp/include/)


target_link_libraries( # Specifies the target library.
                       native-lib
                       simple-mediadata-lib
                       media-lib
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

target_link_libraries( # Specifies the target library.
                       media-lib
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                       )



target_link_libraries(
                        ffmpeg-examples-lib
                        ${log-lib}
                     )








