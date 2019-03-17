# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)


add_library( # Sets the name of the library.
        media-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/include/mediadata.cpp
        )


add_library( # Sets the name of the library.
        codec-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/codec/decode/decode.cpp
        src/main/cpp/codec/decode/decodeUtil.cpp

        src/main/cpp/codec/encode/encode_video.cpp
        src/main/cpp/codec/encode/encode_audio.cpp

        )


add_library(
        basicoperation

        SHARED

        src/main/cpp/basic/ffmpeg_basic_operations.cpp
        src/main/cpp/basic/FFmpegBasic.cpp
        src/main/cpp/common/LogClient.cpp
)

add_library(
        ffmpegcodec

        SHARED

        src/main/cpp/codec/ffmpeg_codec_operation.cpp
        src/main/cpp/codec/FFmpegCodec.cpp
        src/main/cpp/common/LogClient.cpp

)


add_library(avcodec-57-lib SHARED IMPORTED)
set_target_properties(avcodec-57-lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libavcodec-57.so)

add_library(avdevice-57-lib SHARED IMPORTED)
set_target_properties(avdevice-57-lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libavdevice-57.so)

add_library(avfilter-6-lib SHARED IMPORTED)
set_target_properties(avfilter-6-lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libavfilter-6.so)

add_library(avformat-57-lib SHARED IMPORTED)
set_target_properties(avformat-57-lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libavformat-57.so)

add_library(avutil-55-lib SHARED IMPORTED)
set_target_properties(avutil-55-lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libavutil-55.so)

add_library(swresample-2-lib SHARED IMPORTED)
set_target_properties(swresample-2-lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libswresample-2.so)

add_library(swscale-4-lib SHARED IMPORTED)
set_target_properties(swscale-4-lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libswscale-4.so)

add_library(avresample-3-lib SHARED IMPORTED)
set_target_properties(avresample-3-lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libavresample-3.so)

add_library(x264-148-lib SHARED IMPORTED)
set_target_properties(x264-148-lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libx264-148.so)


find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

include_directories(src/main/cpp/include/)
include_directories(src/main/cpp/common/)
include_directories(src/main/cpp/basic/)
include_directories(src/main/cpp/codec/)


target_link_libraries( # Specifies the target library.
        media-lib
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        )

target_link_libraries(
        codec-lib
        avcodec-57-lib
        avdevice-57-lib
        avfilter-6-lib
        avformat-57-lib
        avutil-55-lib
        swresample-2-lib
        swscale-4-lib
        avresample-3-lib
        x264-148-lib
        ${log-lib}
)


target_link_libraries(
        basicoperation
        avcodec-57-lib
        avdevice-57-lib
        avfilter-6-lib
        avformat-57-lib
        avutil-55-lib
        swresample-2-lib
        swscale-4-lib
        avresample-3-lib
        x264-148-lib
        ${log-lib}
)


target_link_libraries(
        ffmpegcodec
        avcodec-57-lib
        avdevice-57-lib
        avfilter-6-lib
        avformat-57-lib
        avutil-55-lib
        swresample-2-lib
        swscale-4-lib
        avresample-3-lib
        x264-148-lib
        ${log-lib}
)