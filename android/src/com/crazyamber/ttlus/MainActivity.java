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
package com.crazyamber.ttlus;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxEditText;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxRenderer;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.os.Bundle;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.FrameLayout;


import com.adsmogo.adview.AdsMogoLayout;
import com.adsmogo.controller.listener.AdsMogoListener;
import com.adsmogo.util.AdsMogoUtil;
import android.widget.RelativeLayout;


public class MainActivity extends Cocos2dxActivity implements AdsMogoListener {
	private Cocos2dxGLSurfaceView mGLView;
	private AdsMogoLayout adsMogoLayoutCode;
	
	private void initAD() 
	{
		/*------------------------------------------------------------*/
		// 初始化AdsMogoLayout 初始化分为以下几种方式
		// 构造方法，设置广告类型，如全屏广告，banner广告
		// public AdsMogoLayout(final Activity context, final String keyAdMogo,
		// final int ad_type) {
		// }

		// 默认的构造方法，默认开启快速模式，banner广告
		// public AdsMogoLayout(final Activity context, final String keyAdMogo)
		// {
		// }

		// 构造方法，设置快速模式
		// public AdsMogoLayout(final Activity context, final String keyAdMogo,
		// boolean expressMode) {
		// }

		// 构造方法，设置广告类型和快速模式
		// public AdsMogoLayout(final Activity context, final String keyAdMogo,
		// final int ad_type, final boolean expressMode) {
		// }
		/*------------------------------------------------------------*/

		// 构造方法，设置快速模式
		adsMogoLayoutCode = new AdsMogoLayout(this,
				"3134b762c5ce433db76f0e09498fdef7", false);

		// 设置监听回调 其中包括 请求 展示 请求失败等事件的回调
		adsMogoLayoutCode.setAdsMogoListener(this);

		/*------------------------------------------------------------*/
		// 通过Code方式添加广告条 本例的结构如下(仅供参考)
		// -RelativeLayout/(FILL_PARENT,FILL_PARENT)
		// |
		// +RelativeLayout/(FILL_PARENT,WRAP_CONTENT)
		// |
		// +AdsMogoLayout(FILL_PARENT,WRAP_CONTENT)
		// |
		// \
		// |
		// \
		/*------------------------------------------------------------*/
		RelativeLayout parentLayput = new RelativeLayout(this);
		RelativeLayout.LayoutParams parentLayputParams = new RelativeLayout.LayoutParams(
				RelativeLayout.LayoutParams.FILL_PARENT,
				RelativeLayout.LayoutParams.FILL_PARENT);
		RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(
				RelativeLayout.LayoutParams.FILL_PARENT,
				RelativeLayout.LayoutParams.WRAP_CONTENT);
		layoutParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM,
				RelativeLayout.TRUE);
		parentLayput.addView(adsMogoLayoutCode, layoutParams);

		this.addContentView(parentLayput, parentLayputParams);
	}
	
	/**
	 * 当用户点击广告*(并且Mogo服务记录点击数)
	 */
	@Override
	public void onClickAd(String arg0) {
		Log.d(AdsMogoUtil.ADMOGO, "-=onClickAd=-");

	}

	/**
	 * 当用户点击了广告关闭按钮时回调(关闭广告按钮功能可以在Mogo的App管理中设置)
	 */
	@Override
	public void onCloseAd() {
		Log.d(AdsMogoUtil.ADMOGO, "-=onCloseAd=-");
	}

	/**
	 * 当用户关闭了下载类型广告的详细界面时回调(广告物料类型为下载广告并且是弹出简介下载的才会有此Dialog)
	 */
	@Override
	public void onCloseMogoDialog() {
		Log.d(AdsMogoUtil.ADMOGO, "-=onCloseMogoDialog=-");
	}

	/**
	 * 所有广告平台请求失败时回调
	 */
	@Override
	public void onFailedReceiveAd() {
		Log.d(AdsMogoUtil.ADMOGO, "-=onFailedReceiveAd=-");

	}

	/**
	 * 当用户点击广告*(真实点击 Mogo不一定在调用此回调时记录点击数)
	 */
	@Override
	public void onRealClickAd() {
		Log.d(AdsMogoUtil.ADMOGO, "-=onRealClickAd=-");

	}

	/**
	 * 请求广告成功时回调
	 */
	@Override
	public void onReceiveAd(ViewGroup arg0, String arg1) {
		Log.d(AdsMogoUtil.ADMOGO, "-=onReceiveAd=-");

	}

	/**
	 * 开始请求广告时回调
	 */
	@Override
	public void onRequestAd(String arg0) {
		Log.d(AdsMogoUtil.ADMOGO, "-=onRequestAd=-");

	}
	
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		
		if (detectOpenGLES20()) {
			// get the packageName,it's used to set the resource path
			String packageName = getApplication().getPackageName();
			super.setPackageName(packageName);
			
            // FrameLayout
            ViewGroup.LayoutParams framelayout_params =
                new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
                                           ViewGroup.LayoutParams.FILL_PARENT);
            FrameLayout framelayout = new FrameLayout(this);
            framelayout.setLayoutParams(framelayout_params);

            // Cocos2dxEditText layout
            ViewGroup.LayoutParams edittext_layout_params =
                new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
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
			
			initAD();
		}
		else {
			Log.d("activity", "don't support gles2.0");
			finish();
		}	
	}
	
	 @Override
	 protected void onPause() {
	     super.onPause();
	     mGLView.onPause();
	 }

	 @Override
	 protected void onResume() {
	     super.onResume();
	     mGLView.onResume();
	 }
	 
	@Override
	protected void onDestroy() {
		// 清除 adsMogoLayout 实例 所产生用于多线程缓冲机制的线程池
		if (adsMogoLayoutCode != null) {
			adsMogoLayoutCode.clearThread();
		}
		super.onDestroy();
	}
	 
	 private boolean detectOpenGLES20() 
	 {
	     ActivityManager am =
	            (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
	     ConfigurationInfo info = am.getDeviceConfigurationInfo();
	     return (info.reqGlEsVersion >= 0x20000);
	 }
		
	static {
		System.loadLibrary("game_frame");
	}
}
