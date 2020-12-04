# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("vplayer")

file(GLOB allCpp *.cpp)

add_library(
        v-player
        SHARED
        ${allCpp}
)


find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)


# 定义ffmpeg总目录的变量
set(FFMPEG ${CMAKE_SOURCE_DIR}/ffmpeg)

# 定义 rtmp总目录的变量
set(RTMP ${CMAKE_SOURCE_DIR}/rtmp)

# 指定ffmpeg里面的库文件目录（静态库）
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${FFMPEG}/lib/${CMAKE_ANDROID_ARCH_ABI}")

# 指定rtmp里面的库文件目录（静态库）
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${RTMP}/lib/${CMAKE_ANDROID_ARCH_ABI}")

# 导入FFmpeg头文件
include_directories(${CMAKE_SOURCE_DIR}/ffmpeg/include)

## 导入FFmpeg库文件
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/ffmpeg/lib/${CMAKE_ANDROID_ARCH_ABI}")
#
## RTMP的库文件
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/rtmp/lib/${CMAKE_ANDROID_ARCH_ABI}")

target_link_libraries(
        v-player

        # 此坑不想去踩了，直接可以忽略顺序，为所欲为的填写即可
        -Wl,--start-group
        avcodec avfilter avformat avutil swresample swscale
        -Wl,--end-group

        ${log-lib}
        z  #存放在NDK目录的 可以去支持FFmpeg ,FFmpeg必须要依赖z
        rtmp
        android #NDK中的支持 能够去渲染屏幕
        OpenSLES #NDK的支持 能够去支持声卡
)


#        ${CMAKE_SOURCE_DIR}/rtmp/lib/${CMAKE_ANDROID_ARCH_ABI}/librtmp.a
#
#        ${CMAKE_SOURCE_DIR}/ffmpeg/lib/${CMAKE_ANDROID_ARCH_ABI}/libavcodec.a
#        ${CMAKE_SOURCE_DIR}/ffmpeg/lib/${CMAKE_ANDROID_ARCH_ABI}/libavfilter.a
#        ${CMAKE_SOURCE_DIR}/ffmpeg/lib/${CMAKE_ANDROID_ARCH_ABI}/libavformat.a
#        ${CMAKE_SOURCE_DIR}/ffmpeg/lib/${CMAKE_ANDROID_ARCH_ABI}/libavutil.a
#        ${CMAKE_SOURCE_DIR}/ffmpeg/lib/${CMAKE_ANDROID_ARCH_ABI}/libswresample.a
#        ${CMAKE_SOURCE_DIR}/ffmpeg/lib/${CMAKE_ANDROID_ARCH_ABI}/libswscale.a