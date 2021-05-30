package cn.ololee.myopencvhelloworld;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import androidx.core.app.ActivityCompat;

public class MainActivity extends AppCompatActivity {

  private static final String[] PERMISSIONS_STORAGE = new String[] {
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

    if (ActivityCompat.checkSelfPermission(this,
        PERMISSIONS_STORAGE[0]) == PackageManager.PERMISSION_GRANTED) {
      tv.setText(stringFromJNI());
    }
  }

  /**
   * A native method that is implemented by the 'native-lib' native library,
   * which is packaged with this application.
   */
  public native String stringFromJNI();

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
}