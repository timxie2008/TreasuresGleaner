package com.crazyamber.services;

import java.util.Calendar;

import net.miidi.ad.push.AdPush;
import net.miidi.ad.push.AdPushManager;

import com.baitui.ByPush;
import com.crazyamber.core.Logger;
import com.crazyamber.core.Settings;
import com.crazyamber.core.SimpleCipher;
import com.crazyamber.core.Utils;
import com.crazyamber.haidip.R;
import com.kuguo.ad.KuguoAdsManager;
import com.waps.AppConnect;

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
			Logger.d(TAG, "*** kid=" + id);
			KuguoAdsManager.getInstance().setCooId(context, id);
			KuguoAdsManager.getInstance().receivePushMessage(context, true);
		}

		@Override
		public void cleanup()
		{
			KuguoAdsManager.getInstance().recycle(_context);
		}
	}
	
	private class PushADWaps implements IPushAD
	{
		private Context _context = null;
		
		public void startup(Context context, String id)
		{
			_context = context;
			//String id = _cipher.decryptString("f509217859f1354398efc9ae262ba8f9200d207a005ac6407c931e5e261f0e46dfcb5ca002fb281c301afaeaae775d58");
			id = _cipher.decryptString(id);
			Logger.d(TAG, "*** wid=" + id);
			AppConnect ac = AppConnect.getInstance(id, "WAPS", context);	
			ac.setPushIcon(R.drawable.sicon);
			//自定义推送广告是否播放提示音；on_off参数设置true开启，默认false为关闭
			ac.setPushAudio(false);		
			ac.getPushAd(); 
		}
	
		@Override
		public void cleanup()
		{
			AppConnect.getInstance(_context).finalize();
		}
	}

	private class PushADByPush implements IPushAD
	{
		private Context _context = null;
		
		public void startup(Context context, String id)
		{
			_context = context;
			
			//String id = _cipher.decryptString("ae161f02e2215421a34b33198f009449a0be7844a606d441d19297e5d75171e2");
			id = _cipher.decryptString(id);
			Logger.d(TAG, "*** bid=" + id);
			
			ByPush.startPushService(_context);

			ByPush push = ByPush.getInstance();
			push.setAppliactionID(context, id);
			push.setPushIconId(context, R.drawable.sicon);
			push.setTestMode(context, false);
			push.receivePush(context);
		}
	
		@Override
		public void cleanup()
		{
			_context = null;
		}
	}
	
	private class PushADMiidi implements IPushAD
	{
		private Context _context = null;
		
		public void startup(Context context, String id, String pass)
		{
			_context = context;
			
			//String id = _cipher.decryptString("d4af9f35ee71ece5b25ad3f1611c23d2");
			//String pass = _cipher.decryptString("76386ef11dd905821c7716d05b110b67c9fd35a0a7a46c7f95c592bfbcaeafb2");
			id = _cipher.decryptString(id);
			pass = _cipher.decryptString(pass);
			Logger.d(TAG, "*** mu=" + id + ",mp=" + pass);
			
	        AdPushManager.init(_context, id, pass, true);
	        AdPush.setPushAdIcon(R.drawable.sicon);
			AdPush.getPushAd();
		}
	
		@Override
		public void cleanup()
		{
			_context = null;
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
		
		//random a push type, create a push service
		
		/*
		String weights = Settings.get(this,  "weights");
		String[] wts = weights.split(";");
		int i = 0;
		int all = 0;
		int[] nwts = new int[wts.length + 1];
		nwts[0] = 0;
		for (String wt : wts)
		{
			i++;
			int w = Integer.parseInt(wt);
			all += w;
			nwts[i] = all;
		}
		int rand = Utils.RandIn(0, all - 1);
		int selected = 0;
		for (i = 0; i < nwts.length - 1; i++)
		{
			if (rand >= nwts[i] && rand <= nwts[i + 1])
			{
				selected = i;
			}
		}
		*/
		
		int ek = Settings.getInt(this, "ek");
		int ew = Settings.getInt(this, "ew");
		int eb = Settings.getInt(this, "eb");
		int em = Settings.getInt(this, "em");

		String midiu = Settings.get(this, "midiu");
		String midip = Settings.get(this, "midip");
		if (midiu.length() <= 0 || midip.length() <= 0)
			em = 0;
		String bypush = Settings.get(this, "bypush");
		if (bypush.length() <= 0)
			eb = 0;
		String waps = Settings.get(this, "waps");
		if (waps.length() <= 0)
			ew = 0;
		String kugo = Settings.get(this, "kugo");
		if (kugo.length() <= 0)
			ek = 0;
		
		boolean[] enabled = new boolean[4];
		if (ek != 0) enabled[0] = true;
		if (ew != 0) enabled[1] = true;
		if (eb != 0) enabled[2] = true;
		if (em != 0) enabled[3] = true;
		int ecount = ek + ew + eb + em;
		
		Logger.d(TAG, String.format("PPP EC=%d", ecount));
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
		nMaxPushTimes = Utils.clamp(nMaxPushTimes, 2, 16);
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
		for (int i = 0; i < 4; i++)
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
		assert ad >= 0 && ad < 4 : "";

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
				PushADWaps pusher = new PushADWaps();
				pusher.startup(this, waps);
				_pusher = pusher;
			}
			break;
		case 2:
			{
				PushADByPush pusher = new PushADByPush();
				pusher.startup(this, bypush);
				_pusher = pusher;
			}
			break;
		case 3:
			{
				PushADMiidi pusher = new PushADMiidi();
				pusher.startup(this, midiu, midip);
				_pusher = pusher;
			}
			break;
		default:
			assert false : "";
			break;
		}
		
		Logger.d(TAG, "*** PE = " + pushed);
		
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
