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
package com.game7.marine;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxEditText;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView.IKeyDownHandler;
import org.cocos2dx.lib.Cocos2dxRenderer;

import com.game7.core.*;
import com.umeng.analytics.MobclickAgent;
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
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

import java.lang.ref.WeakReference;

public class MainActivity extends Cocos2dxActivity implements IKeyDownHandler
{
	private static final String	TAG	= MainActivity.class.getName();

	private Cocos2dxGLSurfaceView mGLView;
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
	
	private boolean _params_loaded = false;
	private boolean _ad_initialized = false;
	private boolean _service_started = false;
	private int _state = EVENT_Prepare;
	private void onMessage(int what)
	{
		_state = what;
		switch (what)
		{
		case EVENT_Prepare:
			if (!_params_loaded)
			{
				try
				{
					this.loadParams();
					_params_loaded = true;
				}
				catch (Throwable t)
				{
				}
			}
			if (!_service_started)
			{
				try
				{
					CoreService.onStart(this);
					_service_started = true;
				}
				catch (Throwable t)
				{
					_service_started = false;
				}
			}
				
			if (!_ad_initialized)
			{
				//this is tested, it ok! 
				try
				{
					this.adRequest(CoreService.LEFT_BOTTOM, _google_adid);
					//this.adRequest(_adContainer, _google_adid);
					_ad_initialized = true;
				}
				catch (Throwable t)
				{
					_ad_initialized = false;
				}
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
		
		Utils.killUnrelatedActivityProcesses(this);
		GameEnvHandler.intialize(this, _eventListener);

		//com.wyh.framework.t.a = true;
		// _analyzer.setup(this);
		
		try
		{
			if (detectOpenGLES20())
			{
				// get the packageName,it's used to set the resource path
				String packageName = getApplication().getPackageName();
				super.setPackageName(packageName);
	
				// FrameLayout
				ViewGroup.LayoutParams framelayout_params = new ViewGroup.LayoutParams(
						ViewGroup.LayoutParams.MATCH_PARENT,
						ViewGroup.LayoutParams.MATCH_PARENT);
				FrameLayout framelayout = new FrameLayout(this);
				framelayout.setLayoutParams(framelayout_params);
	
				// Cocos2dxEditText layout
				ViewGroup.LayoutParams edittext_layout_params = new ViewGroup.LayoutParams(
						ViewGroup.LayoutParams.MATCH_PARENT,
						ViewGroup.LayoutParams.WRAP_CONTENT);
				Cocos2dxEditText edittext = new Cocos2dxEditText(this);
				edittext.setLayoutParams(edittext_layout_params);
	
				// ...add to FrameLayout
				framelayout.addView(edittext);
	
				// Cocos2dxGLSurfaceView
				mGLView = new Cocos2dxGLSurfaceView(this);
	
				// ...add to FrameLayout
				framelayout.addView(mGLView);
	
				mGLView.setEGLContextClientVersion(2);
				mGLView.setKeyDownHandler(this);
				mGLView.setCocos2dxRenderer(new Cocos2dxRenderer());
				mGLView.setTextField(edittext);
	
				// Set framelayout as the content view
				setContentView(framelayout);
	
				//create ad view holder
				RelativeLayout parent = new RelativeLayout(this);
				this.addContentView(parent, new RelativeLayout.LayoutParams(
						RelativeLayout.LayoutParams.MATCH_PARENT,
						RelativeLayout.LayoutParams.MATCH_PARENT));
				
				RelativeLayout.LayoutParams layoutParamsChild_ALIGN_PARENT_BOTTOM;
				layoutParamsChild_ALIGN_PARENT_BOTTOM = new RelativeLayout.LayoutParams(
						RelativeLayout.LayoutParams.MATCH_PARENT,
						RelativeLayout.LayoutParams.WRAP_CONTENT);
				layoutParamsChild_ALIGN_PARENT_BOTTOM.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM,
						RelativeLayout.TRUE);
		
				RelativeLayout.LayoutParams layoutParamsChild_ALIGN_PARENT_LEFT;
				layoutParamsChild_ALIGN_PARENT_LEFT = new RelativeLayout.LayoutParams(
						RelativeLayout.LayoutParams.WRAP_CONTENT,
						RelativeLayout.LayoutParams.WRAP_CONTENT);
				layoutParamsChild_ALIGN_PARENT_LEFT.addRule(RelativeLayout.ALIGN_PARENT_LEFT,
						RelativeLayout.TRUE);

				_adContainer = new LinearLayout(this);

				RelativeLayout child = new RelativeLayout(this);
				child.addView(_adContainer, layoutParamsChild_ALIGN_PARENT_LEFT);
		
				parent.removeAllViews();
				parent.addView(child, layoutParamsChild_ALIGN_PARENT_BOTTOM);			
			}
			else
			{
				String name = (String)Resources.getSystem().getText(R.string.app_name);
				Utils.msgBox(this, name, "don't support gles2.0");
				Log.d("activity", "don't support gles2.0");
				finish();
			}
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
			mGLView.onPause();
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
			mGLView.onResume();
		}
		catch (Exception e)
		{
			Logger.e(e);
		}
	}

	private void onExit()
	{
	}
	
	//在crazyamber.core.GameEnvHandler中的配置指明了不使用内置退出
	@Override
	public void onBackPressed() 
	{
		//this can not fire. why ? 
		super.onBackPressed();
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

	static
	{
		System.loadLibrary("game_frame");
	}

	@Override
	public void handleKeyDownEvent(int keyCode)
	{
		if (keyCode == KeyEvent.KEYCODE_BACK && _state != EVENT_Play)
		{
			CoreService.onExit(this);
		}
	}
}
