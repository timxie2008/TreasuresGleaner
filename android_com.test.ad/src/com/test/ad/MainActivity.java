/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.test.ad;

import com.test.core.*;
import com.umeng.analytics.MobclickAgent;
import com.wyh.framework.BaseActivity;
import com.wyh.framework.CoreService;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.content.res.Resources;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

import java.lang.ref.WeakReference;

public class MainActivity extends BaseActivity
{
	private static final String	TAG	= MainActivity.class.getName();

	private RelativeLayout _adLayout = null;
	private LinearLayout _adContainer = null;
	private static String _google_adid = "32fb5ea6cfc64280";

	private static final int EVENT_Loaded = 0x0001;
	private static final int EVENT_Prepare = 0x0002;
	private static final int EVENT_Play = 0x0003;
	private static final int EVENT_Exit = 0x0004;
	
	private static class _ADManagerHandler extends Handler
	{
		WeakReference<MainActivity> mActivity;  

		_ADManagerHandler(MainActivity activity) 
		{  
			mActivity = new WeakReference<MainActivity>(activity);  
		}  
		@Override
	    public void handleMessage(Message msg) 
		{
           MainActivity theActivity = mActivity.get();
           theActivity.onMessage(msg.what);
	    }
	};
	
	private boolean _ad_initialized = false;
	private int _state = 0;
	private void onMessage(int what)
	{
		_state = what;
		switch (what)
		{
		case EVENT_Prepare:
			if (!_ad_initialized)
			{
				//CoreService.onStart(this);
				
				//this is invisible
				//this.adRequest(CoreService.LEFT_BOTTOM, _google_adid);

				//this is tested, it ok! 
				this.adRequest(_adContainer, _google_adid);
				
				_ad_initialized = true;
			}
			break;
		case EVENT_Play:
			break;
		case EVENT_Exit:
			this.onExit();
			break;
		}
	}
	
	private Handler _adManageHandler = new _ADManagerHandler(this);
	
	private GameEnvHandler.GameEventListener _eventListener = new GameEnvHandler.GameEventListener()
	{
		@Override
		public void onEvent(String evt, String val)
		{
			if (evt.equals("play_state"))
			{
				MobclickAgent.onEvent(MainActivity.this, val);
				if (val.equals("prepare"))
				{
					Message msg = new Message();
					msg.what = EVENT_Prepare;
					_adManageHandler.sendMessage(msg);
				}
				else if (val.equals("running"))
				{
					Message msg = new Message();
					msg.what = EVENT_Play;
					_adManageHandler.sendMessage(msg);
				}
				else if (val.equals("exit"))
				{
					Message msg = new Message();
					msg.what = EVENT_Exit;
					_adManageHandler.sendMessage(msg);
				}
			}
		}
	};


	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		//String v = Utils.getPackageVersion(this);
		
		Logger.start(this,  Logger.ERROR, Logger.DEBUG);
		Logger.d(TAG, "craete");
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		adRequest(CoreService.LEFT_BOTTOM, "32fb5ea6cfc64280"); //"a14fd57c14c4dee");
		
		findViewById(R.id.start).setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				CoreService.onStart(MainActivity.this);

			}
		});
		findViewById(R.id.exit).setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				CoreService.onExit(MainActivity.this);

			}
		});
		try
		{
			this.loadParams();
		}
		catch (Exception e)
		{
			Logger.e(e);
		}
	}

	@Override
	protected void onPause()
	{
		super.onPause();
		try
		{
			CoreService.onMainScreen(this);
			//MobclickAgent.onPause(this);
		}
		catch (Exception e)
		{
			Logger.e(e);
		}
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		try
		{
			MobclickAgent.onResume(this);
		}
		catch (Exception e)
		{
			Logger.e(e);
		}
	}

	private void onExit()
	{
	}
	
	@Override
	public void onBackPressed() 
	{
		//this can not fire. why ? 
		super.onBackPressed();
		CoreService.onExit(this);
	}
	
	@Override
	protected void onDestroy()
	{
		super.onDestroy();
		
		Logger.stop();
	}

	private boolean detectOpenGLES20()
	{
		try
		{
			ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
			ConfigurationInfo info = am.getDeviceConfigurationInfo();
			return (info.reqGlEsVersion >= 0x20000);
		}
		catch (Exception e)
		{
			Logger.e(e);
		}
		return false;
	}
}
