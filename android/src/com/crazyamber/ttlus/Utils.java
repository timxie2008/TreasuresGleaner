package com.crazyamber.ttlus;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.List;

import android.annotation.TargetApi;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Environment;
import android.telephony.TelephonyManager;

public class Utils
{
	//private static final String	TAG	= Utils.class.getSimpleName();

	/**
	 * 检查SD卡是否可用
	 * 
	 * @return
	 */
	public static boolean isSDCardAvailable()
	{
		String status = Environment.getExternalStorageState();
		if (status.equals(Environment.MEDIA_MOUNTED))
		{
			return true;
		}
		return false;
	}

	public static long currentTimeMillis()
	{
		return System.currentTimeMillis();
	}

	public static Integer getAPILevel()
	{
		return android.os.Build.VERSION.SDK_INT;
	}

	public static String getPackageVersion(Context c)
	{
		String strVersion = "";

		PackageManager pm = c.getPackageManager();
		PackageInfo pi;

		try
		{
			pi = pm.getPackageInfo(c.getPackageName(), 0);
			strVersion = pi.versionName;
		}
		catch (Throwable e)
		{
			strVersion = ""; // failed, ignored
		}
		return strVersion;
	}

	public static String map2String(HashMap<String, String> message)
	{
		String r = "";
		for (String key : message.keySet())
		{
			r += "'" + key + "'";
			r += ":";
			r += "'" + message.get(key) + "'; ";
		}
		return r;
	}

	public static String safeString(String s)
	{
		if (null == s) return "";
		return s;
	}

	public static String readStringFromStream(InputStream is) throws Exception
	{
		String recv = "";
		int available = is.available(), len = 0, total = 0;
		if (available > 0)
		{
			byte[] buf = new byte[available];
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			while (available > 0 && total < available)
			{
				len = is.read(buf);
				total += len;
				baos.write(buf, 0, len);
			}
			recv = new String(baos.toByteArray(), "UTF-8");
		}
		return recv;
	}

	public static String getIpAddress()
	{
		try
		{
			for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();)
			{
				NetworkInterface intf = en.nextElement();
				for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();)
				{
					InetAddress inetAddress = enumIpAddr.nextElement();
					if (!inetAddress.isLoopbackAddress())
					{
						return inetAddress.getHostAddress().toString();
					}
				}
			}
		}
		catch (SocketException e)
		{
			Logger.e(e);
		}
		catch (Throwable e)
		{
			Logger.e(e);
		}

		return "";
	}

	private static int	_nid	= 100;

	public static void showNotification(Context c, int Ricon, String title, String message, Intent intent)
	{
		try
		{
			NotificationManager nm = (NotificationManager) c.getSystemService(Context.NOTIFICATION_SERVICE);
			// 新建状态栏通知֪
			Notification baseNF = new Notification();

			// 设置通知在状态栏显示的图标
			baseNF.icon = Ricon; // R.drawable.ic_message;

			// 通知时在状态栏显示的内容
			baseNF.tickerText = "You clicked BaseNF!";

			// 通知的默认参数 DEFAULT_SOUND, DEFAULT_VIBRATE, DEFAULT_LIGHTS.
			// 如果要全部采用默认值, 用 DEFAULT_ALL.
			// 此处采用默认声音
			// baseNF.defaults |= Notification.DEFAULT_SOUND;
			baseNF.defaults |= Notification.DEFAULT_VIBRATE;
			// baseNF.defaults |= Notification.DEFAULT_LIGHTS;

			// 让声音、振动无限循环，直到用户响应
			baseNF.flags |= Notification.FLAG_INSISTENT;

			// 通知被点击后，自动消失
			baseNF.flags |= Notification.FLAG_AUTO_CANCEL;

			// 点击'Clear'时，不清楚该通知(QQ的通知无法清除，就是用的这个)
			baseNF.flags |= Notification.FLAG_NO_CLEAR;

			// 第二个参数 ：下拉状态栏时显示的消息标题 expanded message title
			// 第三个参数：下拉状态栏时显示的消息内容 expanded message text
			// 第四个参数：点击该通知时执行页面跳转ת
			baseNF.setLatestEventInfo(c, title, message, PendingIntent.getActivity(c, 0, intent, PendingIntent.FLAG_CANCEL_CURRENT));

			// 发出状态栏通知
			// The first parameter is the unique ID for the Notification
			// and the second is the Notification object.
			nm.notify(_nid++, baseNF);
		}
		catch (Throwable e)
		{
			Logger.e(e);
		}
	}

	public static void showNotification2(Context c, int Ricon, String title, String message)
	{
		try
		{
			// 1.通过getSystemService方法获得一个NotificationManager对象
			NotificationManager notificationManager = (NotificationManager) c.getSystemService(Context.NOTIFICATION_SERVICE);
			// 2.创建一个Notification对象
			Notification notification = new Notification(Ricon, // R.drawable.ic_message,
			"IESS消息", System.currentTimeMillis());
			// 3.创建PendingIntent对象
			notification.defaults |= Notification.DEFAULT_SOUND;
			notification.defaults |= Notification.DEFAULT_VIBRATE;
			// 让声音、振动无限循环，直到用户响应
			notification.flags |= Notification.FLAG_INSISTENT;

			// 通知被点击后，自动消失
			notification.flags |= Notification.FLAG_AUTO_CANCEL;
			PendingIntent pi = PendingIntent.getActivity(c, 0, null, PendingIntent.FLAG_CANCEL_CURRENT);
			notification.setLatestEventInfo(c, title, message, pi);
			notificationManager.notify(_nid++, notification);
		}
		catch (Throwable e)
		{
			Logger.e(e);
		}
	}

	// first
	// android.os.Build.static vars

	// second
	// add <uses-permission android:name="android.permission.READ_PHONE_STATE"/>
	// first
	public static TelephonyManager getTelephonyManager(Context c)
	{
		TelephonyManager tm = (TelephonyManager) c.getSystemService(Context.TELEPHONY_SERVICE);
		return tm;
	}

	public static boolean isApplicationHasInstalled(Context c, String app)
	{
		PackageManager pm = c.getPackageManager();
		List<PackageInfo> pal = pm.getInstalledPackages(0);
		for (int i = 0, size = pal.size(); i < size; i++)
		{
			PackageInfo p = pal.get(i);
			ApplicationInfo ai = p.applicationInfo;
			String appname = ai.loadLabel(pm).toString();
			// String packagename = p.packageName;
			if ((ai.flags & ApplicationInfo.FLAG_SYSTEM) > 0)
			{
			}
			else
			{
			}
			if (appname.equals(app)) return true;
		}
		return false;
	}

	public static int hashString2Integer(String str)
	{
		MessageDigest messageDigest = null;

		try
		{
			messageDigest = MessageDigest.getInstance("MD5");
			messageDigest.reset();
			messageDigest.update(str.getBytes("UTF-8"));
		}
		catch (NoSuchAlgorithmException e)
		{
			Logger.e(e);
			System.out.println("NoSuchAlgorithmException caught!");
			System.exit(-1);
		}
		catch (UnsupportedEncodingException e)
		{
			Logger.e(e);
			e.printStackTrace();
		}
		catch (Throwable e)
		{
			Logger.e(e);
		}

		int ret = 0x900dface;

		try
		{
			byte[] byteArray = messageDigest.digest();

			int obytes = 4;
			byte[] output = new byte[obytes];

			int i;
			for (i = 0; i < output.length; i++)
			{
				output[i] = 0;
			}
			for (i = 0; i < byteArray.length; i++)
			{
				output[i % obytes] ^= byteArray[i];
			}

			for (i = 0; i < output.length; i++)
			{
				ret <<= 8;
				ret += output[i];
			}
			if ((ret & 0x80000000) != 0) ret ^= 0x8aaaaaaa;
		}
		catch (Throwable e)
		{
			Logger.e(e);
		}

		return ret;
	}

	// 高效的字符替换程序
	public static String replace(String strSource, String strFrom, String strTo)
	{
		if (strSource == null)
		{
			return null;
		}
		int i = 0;
		if ((i = strSource.indexOf(strFrom, i)) >= 0)
		{
			char[] cSrc = strSource.toCharArray();
			char[] cTo = strTo.toCharArray();
			int len = strFrom.length();
			StringBuffer buf = new StringBuffer(cSrc.length);
			buf.append(cSrc, 0, i).append(cTo);
			i += len;
			int j = i;
			while ((i = strSource.indexOf(strFrom, i)) > 0)
			{
				buf.append(cSrc, j, i - j).append(cTo);
				i += len;
				j = i;
			}
			buf.append(cSrc, j, cSrc.length - j);
			return buf.toString();
		}
		return strSource;
	}

	// 自动创建快捷方式
	public static void createShortcut(Context context, String shortcutname, int icon, Class<?> target)
	{
		// <uses-permission
		// android:name="com.android.launcher.permission.INSTALL_SHORTCUT" />
		Intent intent = new Intent("com.android.launcher.action.INSTALL_SHORTCUT");
		intent.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE, Intent.ShortcutIconResource.fromContext(context, icon));
		intent.putExtra(Intent.EXTRA_SHORTCUT_NAME, shortcutname);
		intent.putExtra("duplicate", false);
		Intent sIntent = new Intent(Intent.ACTION_MAIN);
		sIntent.addCategory(Intent.CATEGORY_LAUNCHER);// 加入action,和category之后，程序卸载的时候才会主动将该快捷方式也卸载
		sIntent.setClass(context, target);
		intent.putExtra(Intent.EXTRA_SHORTCUT_INTENT, sIntent);
		context.sendBroadcast(intent);
	}

	// 判断快捷方式是否存在
	public static boolean hasShortCut(Context context, String shortcutname)
	{
		// <uses-permission
		// android:name="com.android.launcher.permission.READ_SETTINGS"/>
		// <uses-permission
		// android:name="com.android.launcher.permission.INSTALL_SHORTCUT"/>
		String url = "";
		System.out.println(getSystemVersion());
		if (getSystemVersion() < 8)
		{
			url = "content://com.android.launcher.settings/favorites?notify=true";
		}
		else
		{
			url = "content://com.android.launcher2.settings/favorites?notify=true";
		}
		ContentResolver resolver = context.getContentResolver();
		Cursor cursor = resolver.query(Uri.parse(url), null, "title= ?", new String[] { shortcutname }, null);

		if (cursor != null && cursor.moveToFirst())
		{
			cursor.close();
			return true;
		}

		return false;
	}

	private static int getSystemVersion()
	{
		return android.os.Build.VERSION.SDK_INT;
	}

	public static void sleep(long ms)
	{
		try
		{
			Thread.sleep(ms);
		}
		catch (InterruptedException e)
		{
			Logger.e(e);
		}
		catch (Throwable e)
		{
			Logger.e(e);
		}

	}

	public static void run(String app, String appparams)
	{
		String[] params = appparams.split(" ");
		String[] progArray = new String[params.length + 1];

		int nIndex = 0;
		progArray[nIndex++] = app;
		for (String p : params)
		{
			progArray[nIndex++] = p;
		}
		try
		{
			// sh = Runtime.getRuntime().exec(progArray);
			Runtime.getRuntime().exec(progArray);
		}
		catch (IOException e)
		{
			Logger.e(e);
		}
		catch (Throwable e)
		{
			Logger.e(e);
		}
	}

	@TargetApi(8)
	public static void killUnrelatedActivityProcesses(Context c)
	{
		ActivityManager am = (ActivityManager) c.getSystemService(Context.ACTIVITY_SERVICE);

		String[] friends = { "com.crazyamber.", "launcher" };
		List<RunningAppProcessInfo> apps = am.getRunningAppProcesses();
		for (RunningAppProcessInfo pi : apps)
		{
			for (String pname : pi.pkgList)
			{
				if (null != pname && pname.length() > 0)
				{
					boolean bFriend = false;
					for (String friend : friends)
					{
						if (pname.contains(friend))
						{
							bFriend = true;
							break;
						}
					}
					if (bFriend)
					{
						continue;
					}
					try
					{
						if (getSystemVersion() >= 8)
						{
							am.killBackgroundProcesses(pname);
						}
					}
					catch (Exception e)
					{
						// TODO Auto-generated catch block
					}
				}
			}
		}
	}

	public static boolean fileExists(String filename)
	{
		File file = new File(filename);
		return file.exists();
	}

	/*
	protected static interface _Function
	{
		void process(final Looper looper, final HandlerThread ht, Message msg);
	};

	// not working now
	// _temp_ui_thread will running forever
	protected static void _runInThread(final Context c, final int what, final Object obj, final _Function funcPrepare)
	{
		new HandlerThread("_temp_ui_thread")
		{
			@Override
			protected void onLooperPrepared()
			{
				new Handler(getLooper())
				{
					@Override
					public void handleMessage(Message msg)
					{
						funcPrepare.process(getLooper(), (HandlerThread) getLooper().getThread(), msg);
					}
				}.obtainMessage(what, obj).sendToTarget();
			}
		}.start();
	}

	public static void toastInThread(final Context c, final String message)
	{
		final int DISPLAY_UI_TOAST = 0x70a37;
		_runInThread(c, DISPLAY_UI_TOAST, message, new _Function()
		{
			@Override
			public void process(final Looper looper, final HandlerThread ht, Message msg)
			{
				if (DISPLAY_UI_TOAST == msg.what)
				{
					Toast t = Toast.makeText(c, (String) msg.obj, Toast.LENGTH_LONG);
					t.show();
					final Timer timer = new Timer();
					timer.schedule(new TimerTask()
					{
						@Override
						public void run()
						{
							ht.quit();
							timer.cancel();
						}
					}, 5000);
				}
			}
		});
	}
	*/
	
	public static int parseInt(String str)
	{
		int ret = 0;
		try
		{
			ret = Integer.valueOf(str);
		}
		catch (Exception e)
		{
			
		}
		return ret;
	}
}
