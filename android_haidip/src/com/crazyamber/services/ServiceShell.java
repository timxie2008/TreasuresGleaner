package com.crazyamber.services;

import com.crazyamber.core.Logger;
import com.crazyamber.core.Utils;

import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Messenger;
import android.widget.RemoteViews;
/**
 * 服务
 * @author niuchao
 *
 */
public class ServiceShell extends Service
{
	private static final String	TAG			= ServiceShell.class.getSimpleName();

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

	// private static int _counter = 0;

	@Override
	public void onCreate()
	{
		Logger.d(TAG, "onCreate\n");

		_safeCheck();

		//this.setForeground(true);

		// _counter++;
	}

	@Override
	public void onStart(Intent intent, int startId)
	{
		if (Utils.getAPILevel() >= 7)
		{
			// onStartCommand will be called
		}
		else
		{
			Logger.d(TAG, "onStart");
			assert null != _getMessenger() : "onCreate should be called";
			_safeCheck();
		}
		return;
	}

	private void _createForgroundStuff()
	{
	}
	
	// @Override
	public int onStartCommand(Intent intent_, int flags, int startId)
	{
		Logger.d(TAG, "onStartCommand(flags=" + flags + ", startId=" + startId);
		
		_createForgroundStuff();
		
		_safeCheck();
		return START_REDELIVER_INTENT;
		//return 1; /* START_STICKY *//* for building API Level 4 package */
	}

	@Override
	public IBinder onBind(Intent intent)
	{
		Logger.d(TAG, "onBind");
		_safeCheck();
		return _getMessenger().getBinder();
	}

	@Override
	public boolean onUnbind(Intent intent)
	{
		Logger.d(TAG, "onUnbind");
		return super.onUnbind(intent);
	}

	@Override
	public void onDestroy()
	{
		Logger.d(TAG, "onDestroy");
		super.onDestroy();
	}
}
