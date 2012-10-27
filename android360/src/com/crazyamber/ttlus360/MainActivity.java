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
package com.crazyamber.ttlus360;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxEditText;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxRenderer;

import com.crazyamber.core.*;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.content.res.Resources;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;

import java.lang.ref.WeakReference;

public class MainActivity extends Cocos2dxActivity
{
	private static final String	TAG	= MainActivity.class.getSimpleName();

	private Cocos2dxGLSurfaceView mGLView;
	private RelativeLayout _adLayout = null;
	
	private ADAdapter _ad = new ADAdapter();
	private AnalyzerAdapter _analyzer = new AnalyzerAdapter();  
	
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
           theActivity._ad.update(msg.what);
	    }
	};
	
	private Handler _adManageHandler = new _ADManagerHandler(this);
	
	private GameEnvHandler.GameEventListener _eventListener = new GameEnvHandler.GameEventListener()
	{
		@Override
		public void onEvent(String evt, String val)
		{
			if (evt.equals("play_state"))
			{
				_analyzer.onEvent(MainActivity.this, val);
				if (val.equals("prepare"))
				{
					Message msg = new Message();
					msg.what = ADAdapter.EVENT_Prepare;
					_adManageHandler.sendMessage(msg);
				}
				if (val.equals("running"))
				{
					Message msg = new Message();
					msg.what = ADAdapter.EVENT_Play;
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
		GameEnvHandler.setListener(this, _eventListener);

		_analyzer.setup(this);
		
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
				mGLView.setCocos2dxRenderer(new Cocos2dxRenderer());
				mGLView.setTextField(edittext);
	
				// Set framelayout as the content view
				setContentView(framelayout);
	
				//create ad view holder
				RelativeLayout parentLayout = new RelativeLayout(this);
				
				_adLayout = parentLayout; 
				this.addContentView(_adLayout, new RelativeLayout.LayoutParams(
						RelativeLayout.LayoutParams.MATCH_PARENT,
						RelativeLayout.LayoutParams.MATCH_PARENT));
			}
			else
			{
				String name = (String)Resources.getSystem().getText(R.string.app_name);
				Utils.msgBox(this, name, "don't support gles2.0");
				Log.d("activity", "don't support gles2.0");
				finish();
			}
			
			_ad.setup(this, _adLayout, _analyzer);
			//_ad.prepare();
			_ad.update(ADAdapter.EVENT_Loaded);
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
			_analyzer.pause(this);
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
			_analyzer.resume(this);
			mGLView.onResume();
		}
		catch (Exception e)
		{
			Logger.e(e);
		}
	}

	@Override
	protected void onDestroy()
	{
		super.onDestroy();
		try
		{
			_ad.cleanup();
		}
		catch (Exception e)
		{
			Logger.e(e);
		}
		
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
}
