package cn.ololee.myopencvhelloworld

import android.graphics.Bitmap

class FaceDetection {

    external fun loadCascade(path:String)
    external fun faceDetectSave(bitmap: Bitmap):Int
}