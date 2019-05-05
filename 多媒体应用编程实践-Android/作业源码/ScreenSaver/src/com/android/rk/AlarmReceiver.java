package com.android.rk;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class AlarmReceiver extends BroadcastReceiver {
	
	@Override
	public void onReceive(Context context, Intent intent) {
		// TODO Auto-generated method stub
		
		Log.d(AlarmAlertWakeLock.TAG,"AlarmReceiver -------------------------------------");
		
		AlarmAlertWakeLock.acquire(context);
		
		//Toast.makeText(context, "提示：时间到！", Toast.LENGTH_LONG).show(); 		   	
		Intent nintent = new Intent() ;
		nintent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		nintent.setClass(context, ScreenShow.class);
		
		context.startActivity(nintent);
	   
	}

}
