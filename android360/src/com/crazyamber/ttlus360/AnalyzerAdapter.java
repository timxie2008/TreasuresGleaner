package com.crazyamber.ttlus360;

import android.content.Context;

import com.umeng.analytics.MobclickAgent;
import com.umeng.update.UmengDownloadListener;
import com.umeng.update.UmengUpdateAgent;
import com.umeng.update.UmengUpdateListener;
import com.umeng.update.UpdateResponse;

public class AnalyzerAdapter
{
	//private static final String	TAG	= "AA";
	
	private Context _context = null;
	
	public AnalyzerAdapter()
	{
	}
	
	public void setup(Context c)
	{
		_context = c;
		
		MobclickAgent.setSessionContinueMillis(60000 * 10);
		MobclickAgent.setDebugMode(false);
		MobclickAgent.onError(c);
		MobclickAgent.updateOnlineConfig(c);

		com.umeng.common.Log.LOG = false;

		UmengUpdateAgent.setUpdateOnlyWifi(false); // 目前我们默认在Wi-Fi接入情况下才进行自动提醒。如需要在其他网络环境下进行更新自动提醒，则请添加该行代码
		UmengUpdateAgent.setUpdateAutoPopup(false);
		UmengUpdateAgent.setUpdateListener(new UmengUpdateListener()
		{
			@Override
			public void onUpdateReturned(int updateStatus, UpdateResponse updateInfo)
			{
				switch (updateStatus)
				{
				case 0: // has update
					UmengUpdateAgent.showUpdateDialog(_context, updateInfo);
					break;
				case 1: // has no update
					break;
				case 2: // none wifi
					break;
				case 3: // time out
					break;
				}

			}
		});

		UmengUpdateAgent.setOnDownloadListener(new UmengDownloadListener()
		{
			@Override
			public void OnDownloadEnd(int result)
			{
				//"download result : " + result
			}

		});

		UmengUpdateAgent.update(c);
	}
	
	public void pause(Context c)
	{
		MobclickAgent.onPause(c);
	}
	
	public void resume(Context c)
	{
		MobclickAgent.onResume(c);
	}
	
	public void onEvent(Context c, String e)
	{
		MobclickAgent.onEvent(c, e);
	}
	
	public String getConfigParams(String channel)
	{
		String config = MobclickAgent.getConfigParams(_context, channel);
		return config;
	}
}
