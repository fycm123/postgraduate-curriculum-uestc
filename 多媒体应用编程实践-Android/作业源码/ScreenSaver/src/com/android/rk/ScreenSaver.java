package com.android.rk;



import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;


public class ScreenSaver extends Activity {
	
	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        /*启动 待机屏保 服务*/
        Intent i = new Intent(this,ScreenSaverService.class);
        i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        startService(i);
              
    }
}
