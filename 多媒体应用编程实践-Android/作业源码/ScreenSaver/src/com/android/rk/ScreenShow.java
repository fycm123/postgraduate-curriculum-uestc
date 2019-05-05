package com.android.rk;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;


public class ScreenShow extends Activity {
		
	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
    	/*
    	 * È«ÆÁÏÔÊ¾
    	 */
        getWindow().setFlags(WindowManager.LayoutParams. FLAG_FULLSCREEN ,
        		WindowManager.LayoutParams. FLAG_FULLSCREEN);
        requestWindowFeature(Window.FEATURE_NO_TITLE); 

        setContentView(R.layout.screenshow);
        
    }

	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		   	   		
		//AlarmAlertWakeLock.release();
		Intent i = new Intent("ScreenShow");
        sendBroadcast(i);
        
        AlarmAlertWakeLock.release() ;
        
		Log.d(AlarmAlertWakeLock.TAG,"-------->req:release||Broadcast: ScreenShow");
	}

	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		
		Intent i = new Intent("NewScreenSaver");
		sendBroadcast(i);
        Log.d(AlarmAlertWakeLock.TAG,"------->onStop req:NewScreenSaver");
		finish() ;
	}
	
}
