cmake_minimum_required(VERSION 3.10.2)
project("myopencvhelloworld")
add_library(native-lib SHARED native-lib.cpp facedetect-lib.cpp)
#导入头文件
include_directories(include)

#导入库文件
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}")

find_library(log-lib log )
target_link_libraries(
        native-lib
        android
        opencv_java4
        jnigraphics
        ${log-lib} )