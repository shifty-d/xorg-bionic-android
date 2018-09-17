package org.x.android;

import android.app.NativeActivity;

public class MainActivity extends NativeActivity {
  static { System.loadLibrary("Xorg"); }
}
