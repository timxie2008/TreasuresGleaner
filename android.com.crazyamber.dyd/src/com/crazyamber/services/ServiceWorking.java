package com.crazyamber.services;

import java.util.Calendar;

import com.crazyamber.core.Logger;
import com.crazyamber.core.Settings;
import com.crazyamber.core.SimpleCipher;
import com.crazyamber.core.Utils;
import com.crazyamber.dyd.R;
import com.daoyoudao.push.DydsAdPush;
import com.kuguo.ad.KuguoAdsManager;

import android.app.AlarmManager;
import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.os.Messenger;
import android.widget.RemoteViews;

public class ServiceWorking extends Service
{
	private static final String	TAG			= ServiceWorking.class.getName();
	
	private SimpleCipher _cipher = null;
	
	private interface IPushAD
	{
		//public void startup(Context context);
		public void cleanup();
	}
	
	private IPushAD _pusher = null;
	
	private class PushADKuguo implements IPushAD
	{
		private Context _context = null;
		
		public void startup(Context context, String id)
		{
			_context = context;
			
			//String id = _cipher.decryptString("18d926343d84acdd671d3542fb1bd883cb03810a75ab41e554c892e69b209b9b9a80e0d8d1e06f82793f5c3e7412f457");
			id = _cipher.decryptString(id);
			//id = "f946b3d4086249a6968aabec7c752027"; testid
			Logger.d(TAG, "### kid=" + id);
			KuguoAdsManager.getInstance().setCooId(context, id);
			KuguoAdsManager.getInstance().receivePushMessage(context, true);
		}

		@Override
		public void cleanup()
		{
			KuguoAdsManager.getInstance().recycle(_context);
		}
	}
	
	private class PushADDYD implements IPushAD
	{
		private Context _context = null;
		
		public void startup(Context context, String id)
		{
			_context = context;
			Logger.d(TAG, "### did=" + id);
			DydsAdPush p = DydsAdPush.getInstance(_context);
			p.receiveMessage(1, 40);
		}
	
		@Override
		public void cleanup()
		{
		}
	}

	private void _do()
	{
		Logger.d(TAG, "wroking");

		if (!Utils.isNetworkAvailable(this, true))
		{
			Logger.d(TAG, "NNTW");
			return;
		}
		if (null != _pusher)
		{
			_pusher.cleanup();
			_pusher = null;
			Logger.d(TAG, "clean up old pusher");
			//return;
		}
		
		int ek = Settings.getInt(this, "ek");
		int ed = Settings.getInt(this, "ed");

		String dyd = Settings.get(this, "dyd");
		if (dyd.length() <= 0)
		{
			Logger.d(TAG, "ed = 0");
			ed = 0;
		}
		String kugo = Settings.get(this, "kugo");
		if (kugo.length() <= 0)
		{
			Logger.d(TAG, "ek = 0");
			ek = 0;
		}
		
		boolean[] enabled = new boolean[4];
		if (ek != 0) enabled[0] = true;
		if (ed != 0) enabled[1] = true;
		int ecount = ek + ed;
		
		Logger.d(TAG, String.format("PPP EC=%d, ed=%d, ek=%d", ecount, ed, ek));
		if (ecount <= 0)
		{
			Logger.d(TAG, "DAdALL");
			return;
		}
		
		int hourMin = Settings.getInt(this, "hmin");
		int hourMax = Settings.getInt(this, "hmax");
		if (hourMax < hourMin + 2)
		{
			//Logger.d(TAG, "");
			hourMin = 6;
			hourMax = 22;
		}
		Logger.d(TAG, String.format("hmin=%d,hmax=%d", hourMin, hourMax));

		Calendar cNow = Calendar.getInstance();
		cNow.setTimeInMillis(System.currentTimeMillis());

		int hnow = cNow.get(Calendar.HOUR_OF_DAY);
		//in our hours range?
		if (hnow >= hourMin && hnow < hourMax)
		{
		}
		else
		{
			Logger.d(TAG, "HNOTIN");
			return;
		}

		//default is 8, max push count in one day
		int nMaxPushTimes = Settings.getInt(this, "pmaxcount");
		nMaxPushTimes = Utils.clamp(nMaxPushTimes, 2, 256);
		Logger.d(TAG, String.format("nMaxPushTimes=%d", nMaxPushTimes));
		
		String lp = Settings.get(this, "pdate");
		long lastPushTime = Utils.parseLong(lp);
		Calendar cLast = Calendar.getInstance();
		cLast.setTimeInMillis(lastPushTime);

		int dnow = cNow.get(Calendar.DAY_OF_YEAR);
		int dlast = cLast.get(Calendar.DAY_OF_YEAR);
		
		int pushed = Settings.getInt(this, "pcount");
		Logger.d(TAG, String.format("pcount=%d, dnow=%d,dlast=%d", pushed, dnow, dlast));
		if (dnow >= dlast + 1)
		{
			pushed = 0;
		}
		//min push interval in minutes 
		int minmin = Settings.getInt(this, "pminmin");
		minmin = Utils.clamp(minmin, 5, 20 * 60);

		Logger.d(TAG, String.format("pushed=%d,pminmin=%d", pushed, minmin));
		
		long diff = cNow.getTimeInMillis() - cLast.getTimeInMillis();
		if (diff > minmin * 60 * 1000)
		{
		}
		else
		{
			//it is too close to push another 
			Logger.d(TAG, String.format("2CLS minmin=%d,offs=%d", minmin, diff));
			return;
		}
		if (pushed > nMaxPushTimes)
		{
			Logger.d(TAG, String.format("2MNY ped=%d,pma=%d", pushed, nMaxPushTimes));
			return;
		}
		
		_cipher = new SimpleCipher(
				Utils.hexToBytes("37ca9a53c87eaa66f8fe88444f02a876"), 
				Utils.hexToBytes("a3bb37149550b256009d23fc34b85836"));

		int ad = -1;
		int idx = pushed % ecount;
		for (int i = 0; i < 2; i++)
		{
			if (enabled[i])
			{	
				idx--;
				if (idx < 0)
				{
					ad = i;
					break;
				}
			}
		}
		assert idx < 0 : "";
		assert ad >= 0 && ad < 2 : "";

		Logger.d(TAG, String.format("PPP PB %d, ad=%d", pushed, ad));
		
		switch (ad)
		{
		case 0:
			{
				PushADKuguo pusher = new PushADKuguo();
				pusher.startup(this, kugo);
				_pusher = pusher;
			}
			break;
		case 1:
			{
				PushADDYD pusher = new PushADDYD();
				pusher.startup(this, "disabled");
				_pusher = pusher;
			}
			break;
		default:
			assert false : "";
			break;
		}
		
		Logger.d(TAG, "### PE = " + pushed);
		
		pushed++;
		Settings.set(this, "pdate", Long.toString(cNow.getTimeInMillis()));
		Settings.setInt(this, "pcount", pushed);
	}

	@Override
	public void onCreate()
	{
		Logger.d(TAG, "onCreate");
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
			try
			{
				_do();
			}
			catch (Throwable e)
			{
				Logger.e(TAG, e);
			}
		}
		return;
	}

	// @Override
	public int onStartCommand(Intent intent, int flags, int startId)
	{
		int ret = super.onStartCommand(intent, flags, startId);

		try
		{
			_do();
		}
		catch (Throwable e)
		{
			Logger.e(TAG, e);
		}
		return ret;
		//return START_REDELIVER_INTENT;
		//return 1; /* START_STICKY *//* for building API Level 4 package */
	}

	@Override
	public IBinder onBind(Intent intent)
	{
		return null;
	}

	@Override
	public boolean onUnbind(Intent intent)
	{
		return super.onUnbind(intent);
	}

	@Override
	public void onDestroy()
	{
		if (null != _pusher)
		{
			_pusher.cleanup();
			_pusher = null;
		}
		super.onDestroy();
	}
}
