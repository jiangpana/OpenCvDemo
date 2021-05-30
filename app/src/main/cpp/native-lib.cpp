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