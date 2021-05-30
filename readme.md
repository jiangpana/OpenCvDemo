> 这是一篇笔记,关于怎样在android中使用opencv.
需要准备的资料:
> Android Studio</br>
> [opencv官网](https://opencv.org/releases/)下载对应版本的android sdk
### 步骤一
> 使用现有的C++工程似乎是最快的建立ndk工程的方式

![开一个ndk工程](https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/8be2d890ae7f4869b11b5e8e9d90f8ed~tplv-k3u1fbpfcp-watermark.image)

### 步骤二
> 将`OpenCV-android-sdk\sdk\native\libs`目录下的动态链接库复制到自己的工程下的`\app\libs`或者`\app\src\main\cpp\libs\`下去

![步骤二](https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/5a7738bc927442cb907e6743a93da00a~tplv-k3u1fbpfcp-watermark.image)

### 步骤三
> 将`OpenCV-android-sdk\sdk\native\jni\include`下所有的头文件放入自己工程下的`\app\src\main\cpp\`中去

![步骤三](https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/29c042107d8b410a8b636b3ff32a92ca~tplv-k3u1fbpfcp-watermark.image)

### 步骤四
> 配置gradle
```.java
android {
  ...
  defaultConfig {
   ...
    externalNativeBuild {
      cmake {
        cppFlags ""
        abiFilters 'arm64-v8a'
        arguments "-DANDROID_STL=c++_shared"//这个很重要
      }
    }
   ...
  }

 
 
  sourceSets {
    main {
      jniLibs.srcDirs = ['libs'] //jniLibs.srcDirs = ['src/main/cpp/libs']
    }
  }
  compileOptions {
    sourceCompatibility JavaVersion.VERSION_1_8
    targetCompatibility JavaVersion.VERSION_1_8
  }
  buildToolsVersion '28.0.3'
  ndkVersion '22.1.7171670'
}
dependencies {
    ...
 implementation fileTree(dir: 'libs', include: ['*.jar'])
    ...
 }
```

### 步骤五
> 修改CMakeLists.txt
```txt
include_directories(include)

#导入库文件
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}")

find_library(log-lib log )
target_link_libraries( native-lib android opencv_java4 ${log-lib} )
```

![步骤五](https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/0d74e03d03eb4c7cb4af0dd0371c8544~tplv-k3u1fbpfcp-watermark.image)

### 步骤六
> 做到这里，这个项目基本上opencv的环境就已经配置好了，接下来就可以写个demo验证配置环境有没有搞好。

![cpp](https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/aefdec1efb3943198e8e3a0204a69832~tplv-k3u1fbpfcp-watermark.image)
>c++的代码
```cpp
#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

extern "C" JNIEXPORT jstring JNICALL
Java_cn_ololee_myopencvhelloworld_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    string hello = "文件已经保存至";
    char* result=new char[100];
    string opencvOutPath="/sdcard/img_gray.jpg";
    Mat mat=imread("/sdcard/img.jpg");
    Mat grayMat;
    cvtColor(mat,grayMat,COLOR_BGR2GRAY);
    imwrite(opencvOutPath,grayMat);
    strcat(result,hello.c_str());
    strcat(result,opencvOutPath.c_str());
    jstring return_str=env->NewStringUTF(result);
    delete result;
    return return_str;
}
```
别忘了在Manifest申请读写外置存储器的权限和在活动里动态申请读写文件的权限