package com.crazyamber.ttlus;

import java.util.HashMap;

public class GameEventHandler
{
	public interface GameEventListener
	{
		public void onEvent(String evt, String val);
	}

	private static HashMap<String, Object> _events = new HashMap<String, Object>();
	private static GameEventListener _listener  = null;
	public static void setListener(GameEventListener gel)
	{
		_listener = gel;
	}
	public static void onJNIEvent(String key, String val)
	{
		_events.put(key, val);
		if (null != _listener)
		{
			_listener.onEvent(key,  val);
		}
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
