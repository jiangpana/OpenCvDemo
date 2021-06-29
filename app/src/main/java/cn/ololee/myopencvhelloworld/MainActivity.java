package cn.ololee.myopencvhelloworld;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Handler;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.core.app.ActivityCompat;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private static final String[] PERMISSIONS_STORAGE = new String[]{
            Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE
    };
    private static final int REQUEST_EXTERNAL_STORAGE = 10;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        verifyStoragePermissions(this);

        TextView tv = findViewById(R.id.sample_text);
        ImageView iv1 = findViewById(R.id.iv1);
        Bitmap bitmap1 = BitmapFactory.decodeResource(getResources(), R.mipmap.img);
        Bitmap bitmap2 = Bitmap.createBitmap(bitmap1);
        cannyCheck(bitmap1, bitmap2);
        iv1.setImageBitmap(bitmap2);
        if (ActivityCompat.checkSelfPermission(this,
                PERMISSIONS_STORAGE[0]) == PackageManager.PERMISSION_GRANTED) {
            tv.setText(stringFromJNI());
            detectFace();
        }
        uuid();
        staticUUID();
    }

    private void detectFace() {
        InputStream is = getResources().openRawResource(R.raw.lbpcascade_frontalface);
        File cascadeDir = getDir("cascade", Context.MODE_PRIVATE);
        File cascadeFile =new File(cascadeDir, "lbpcascade_frontalface.xml");
        try {
            FileOutputStream os = new FileOutputStream(cascadeFile);
            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead =is.read(buffer) )!= -1){
                os.write(buffer,0,bytesRead);
            }
            is.close();
            os.close();

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        FaceDetection faceDetection = new FaceDetection();
        faceDetection.loadCascade(cascadeFile.getAbsolutePath());
        new Handler().postDelayed(() -> faceDetection.faceDetectSave(BitmapFactory.decodeResource(getResources(), R.mipmap.mayun)), 2000);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void cannyCheck(Bitmap src, Bitmap dst);


    //然后通过一个函数来申请
    public static void verifyStoragePermissions(Activity activity) {
        try {
            //检测是否有写的权限
            int permission = ActivityCompat.checkSelfPermission(activity,
                    PERMISSIONS_STORAGE[0]);
            if (permission != PackageManager.PERMISSION_GRANTED) {
                // 没有写的权限，去申请写的权限，会弹出对话框
                ActivityCompat.requestPermissions(activity, PERMISSIONS_STORAGE, REQUEST_EXTERNAL_STORAGE);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    public native void uuid();
    private native String staticUUID();

    public String getUUID() {
        return UUID.randomUUID().toString();
    }

    private static String getStaticUUID() {
        return UUID.randomUUID().toString();
    }


}