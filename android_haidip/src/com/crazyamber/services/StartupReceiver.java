/**
 * 
 */
package com.crazyamber.services;

/**
 * 广播接收器
 */
import com.crazyamber.core.Logger;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class StartupReceiver extends BroadcastReceiver
{
	protected String TAG	= StartupReceiver.class.getSimpleName();

	protected void _setTag(String tag)
	{
		TAG = tag;
	}
	
	@Override
	public void onReceive(final Context context, Intent intent) // 回调函数
	{
		try
		{
			Logger.d(TAG, "onReceive");

			// startService
			Intent i = new Intent(context, ServiceShell.class);
			context.startService(i);
			Logger.d(TAG, "start ServiceShell");
		}
		catch (Throwable e)
		{
			Logger.e(e);
		}
	}
}
