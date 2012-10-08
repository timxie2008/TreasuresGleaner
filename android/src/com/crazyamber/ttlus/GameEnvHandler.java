package com.crazyamber.ttlus;

import java.util.HashMap;
import java.util.Locale;

import android.content.Context;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.util.Log;

public class GameEnvHandler
{
	public interface GameEventListener
	{
		public void onEvent(String evt, String val);
	}

	private static HashMap<String, Object> _events = new HashMap<String, Object>();
	private static GameEventListener _listener  = null;
	private static Context _context = null;
	public static void setListener(Context c, GameEventListener gel)
	{
		_context = c;
		_listener = gel;
	}
	public static String getLanguage()
	{
		Resources resources = _context.getResources();//获得res资源对象
		
		Configuration config = resources.getConfiguration();//获得设置对象
		//DisplayMetrics dm = resources .getDisplayMetrics();//获得屏幕参数：主要是分辨率，像素等。
		if (config.locale == Locale.SIMPLIFIED_CHINESE) //简体中文
		{
			return "cn";
		}
		return "en";
	}
	
	public static void onJNIEvent(String key, String val)
	{
		_events.put(key, val);
		if (null != _listener)
		{
			_listener.onEvent(key,  val);
		}
		Log.d("gameevent", val);
	}
	
	public static Object getEvent(String key)
	{
		if (_events.containsKey(key))
		{
			return _events.get(key);
		}
		return null;
	}
}
