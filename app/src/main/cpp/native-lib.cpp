#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <android/bitmap.h>
#include <android/log.h>

#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)

using namespace cv;
using namespace std;

extern "C" JNIEXPORT jstring JNICALL
Java_cn_ololee_myopencvhelloworld_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    string hello = "文件已经保存至";
    char *result = new char[100];
    string opencvOutPath = "/sdcard/img_gray.jpg";
    Mat mat = imread("/sdcard/img.jpg");
    Mat grayMat;
    cvtColor(mat, grayMat, COLOR_BGR2GRAY);
    imwrite(opencvOutPath, grayMat);
    strcat(result, hello.c_str());
    strcat(result, opencvOutPath.c_str());
    jstring return_str = env->NewStringUTF(result);
    delete result;
    return return_str;
}



extern "C" {
JNIEXPORT void
JNICALL
Java_cn_ololee_myopencvhelloworld_MainActivity_cannyCheck(JNIEnv *env, jobject type, jobject src,
                                                          jobject dst);
// bitmap -> mat
Mat bitmap2Mat(jobject pJobject);
// mat -> bitmap
void mat2bitmap(JNIEnv *env, Mat mat, jobject bitmap);
}


Mat bitmap2Mat(JNIEnv *env, jobject bitmap) {
    // 1. 获取图片的宽高，以及格式信息
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);
    void *pixels;
    AndroidBitmap_lockPixels(env, bitmap, &pixels);

    Mat mat;

    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGD("nMatToBitmap: CV_8UC4 -> RGBA_8888");
        mat = Mat(info.height, info.width, CV_8UC4, pixels);
    } else if (info.format = ANDROID_BITMAP_FORMAT_RGB_565) {
        LOGD("nMatToBitmap: CV_8UC2 -> RGBA_565");
        mat = Mat(info.height, info.width, CV_8UC2, pixels);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
    return mat;
}

void mat2bitmap(JNIEnv *env, Mat src, jobject bitmap) {
    // 1. 获取图片的宽高，以及格式信息
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);
    void *pixels;
    AndroidBitmap_lockPixels(env, bitmap, &pixels);

    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat tmp(info.height, info.width, CV_8UC4, pixels);
        if (src.type() == CV_8UC1) {
            LOGD("nMatToBitmap: CV_8UC1 -> RGBA_8888");
            cvtColor(src, tmp, COLOR_GRAY2RGBA);
        } else if (src.type() == CV_8UC3) {
            LOGD("nMatToBitmap: CV_8UC3 -> RGBA_8888");
            cvtColor(src, tmp, COLOR_RGB2RGBA);
        } else if (src.type() == CV_8UC4) {
            LOGD("nMatToBitmap: CV_8UC4 -> RGBA_8888");
            src.copyTo(tmp);
        }
    } else {
        // info.format == ANDROID_BITMAP_FORMAT_RGB_565
        Mat tmp(info.height, info.width, CV_8UC2, pixels);
        if (src.type() == CV_8UC1) {
            LOGD("nMatToBitmap: CV_8UC1 -> RGB_565");
            cvtColor(src, tmp, COLOR_GRAY2BGR565);
        } else if (src.type() == CV_8UC3) {
            LOGD("nMatToBitmap: CV_8UC3 -> RGB_565");
            cvtColor(src, tmp, COLOR_RGB2BGR565);
        } else if (src.type() == CV_8UC4) {
            LOGD("nMatToBitmap: CV_8UC4 -> RGB_565");
            cvtColor(src, tmp, COLOR_RGBA2BGR565);
        }
    }

    AndroidBitmap_unlockPixels(env, bitmap);
}


JNIEXPORT void JNICALL
Java_cn_ololee_myopencvhelloworld_MainActivity_cannyCheck(JNIEnv *env, jobject type, jobject src,
                                                          jobject dst) {
    // 1. bitmap2Mat
    Mat src_mat = bitmap2Mat(env, src);
    Mat gray_mat, dst_mat;
    // 2. 讲图片处理成 Gray 可以提升处理速度
    cvtColor(src_mat, gray_mat, COLOR_BGRA2GRAY);
    // 2.2 3X3降噪处理
    blur(gray_mat, gray_mat, Size(3, 3));
    // 3. 处理边缘检测
    Canny(gray_mat, dst_mat, 50, 30);
    // 4. mat2bitmap
    mat2bitmap(env, dst_mat, dst);
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_ololee_myopencvhelloworld_MainActivity_uuid(JNIEnv *env, jobject jobj) {

    jclass clas = env->GetObjectClass(jobj);
    jmethodID jmID = env->GetMethodID(clas, "getUUID", "()Ljava/lang/String;");
    jstring jstr = (jstring) env->CallObjectMethod(jobj, jmID);
    //将jni中的字符转成c中的字符
    const char *uuid_str = env->GetStringUTFChars(jstr, NULL);
    //这条命令是在LOGCAT中打印（可忽略）
    __android_log_print(ANDROID_LOG_INFO, "JNITag", "%s", uuid_str);

}

extern "C"
JNIEXPORT jstring JNICALL
Java_cn_ololee_myopencvhelloworld_MainActivity_staticUUID(JNIEnv *env, jobject jobj) {
    jclass clas =env->GetObjectClass(jobj);
    jmethodID  jmethodId =env->GetStaticMethodID(clas,"getStaticUUID", "()Ljava/lang/String;");
    jstring  jstr = (jstring)env->CallStaticObjectMethod(clas,jmethodId);
    const char *uuid_str =env->GetStringUTFChars(jstr, NULL);
    __android_log_print(ANDROID_LOG_INFO, "JNITag", "%s", uuid_str);
    return jstr;
}