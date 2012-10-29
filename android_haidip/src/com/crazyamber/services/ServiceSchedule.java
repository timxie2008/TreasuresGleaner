package com.crazyamber.services;

import java.util.Calendar;

import com.crazyamber.core.Logger;
import com.crazyamber.core.Settings;
import com.crazyamber.core.Utils;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Messenger;

public class ServiceSchedule extends Service
{
	private static final String TAG = ServiceSchedule.class.getName();

	//this is action for alarm 
	public static final String ____ = "com.google.services.palm";
	
	private static Messenger	_messenger	= null;
	private Messenger _getMessenger()
	{
		if (null == _messenger)
		{
			ServiceHandler si = new ServiceHandler(this);
			_messenger = new Messenger(si);
		}
		return _messenger;
	}
	private void _safeCheck()
	{
		_getMessenger();
	}

	@Override
	public void onCreate()
	{
		//Logger.d(TAG, "onCrate");
		_safeCheck();
		//Logger.d(TAG, "onCrated");
	}

	private void _scheduleAlarm()
	{
		//Logger.d(TAG, "_scheduleAlarm");
		
		AlarmManager am = (AlarmManager)this.getSystemService(Service.ALARM_SERVICE);
		Calendar c = Calendar.getInstance();
		c.setTimeInMillis(System.currentTimeMillis());

		int ho = c.get(Calendar.HOUR_OF_DAY);
		int mi = c.get(Calendar.MINUTE);
		int se = c.get(Calendar.SECOND);
		
		//Logger.d(TAG, String.format("now %d:%d:%d", ho, mi, se));
		
		//mi += Utils.RandIn(5, 10);
		//if (mi >= 60) { mi = 0; if (ho < 24) ho++; };
		//se = Utils.RandIn(10, 50);

		int sche_secs = 10;
		se += sche_secs;
		mi += se / 60;
		se %= 60;

		//Logger.d(TAG, String.format("will %d:%d:%d", ho, mi, se));
		
		c.set(Calendar.HOUR_OF_DAY, ho);
		c.set(Calendar.MINUTE, mi);
		c.set(Calendar.SECOND, se);

		//Logger.d(TAG, String.format("schedule %d:%d:%d", ho, mi, se));
		
		//设置消息的响应
		Intent ia = new Intent(this, ReceiverCommon.class);
		ia.setAction(____);
		PendingIntent pii = PendingIntent.getBroadcast(this, 0, ia, 0);
		am.set(AlarmManager.RTC_WAKEUP, c.getTimeInMillis(), pii);
	}

	@Override
	public void onStart(Intent intent, int startId)
	{
		super.onStart(intent, startId);
		
		if (Utils.getAPILevel() >= 7)
		{
			// onStartCommand will be called
		}
		else
		{
			_safeCheck();
			_scheduleAlarm();
		}
		return;
	}
	
	// @Override
	public int onStartCommand(Intent intent, int flags, int startId)
	{
		int ret = super.onStartCommand(intent, flags, startId);
		
		//Logger.d(TAG, "onStartCommand(flags=" + flags + ", startId=" + startId);

		_safeCheck();
		_scheduleAlarm();
		
		return ret;
		//return START_REDELIVER_INTENT;
		//return 1; /* START_STICKY *//* for building API Level 4 package */
	}

	@Override
	public IBinder onBind(Intent intent)
	{
		_safeCheck();
		return _getMessenger().getBinder();
	}

	@Override
	public boolean onUnbind(Intent intent)
	{
		return super.onUnbind(intent);
	}

	@Override
	public void onDestroy()
	{
		super.onDestroy();
	}
}
