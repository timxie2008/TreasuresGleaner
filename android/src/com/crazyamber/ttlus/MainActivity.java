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
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.adsmogo.adview.AdsMogoLayout;
import com.adsmogo.controller.listener.AdsMogoListener;
import com.adsmogo.util.AdsMogoUtil;
import com.kuguo.ad.KuguoAdsManager;
import com.umeng.analytics.MobclickAgent;
import com.umeng.update.UmengDownloadListener;
import com.umeng.update.UmengUpdateAgent;
import com.umeng.update.UmengUpdateListener;
import com.umeng.update.UpdateResponse;

import android.widget.RelativeLayout;

import java.lang.ref.WeakReference;
import java.security.NoSuchAlgorithmException;

import javax.crypto.Cipher;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class MainActivity extends Cocos2dxActivity implements AdsMogoListener
{
	private static final String	TAG	= MainActivity.class.getSimpleName();

	private Cocos2dxGLSurfaceView mGLView;
	private AdsMogoLayout _adsMogoLayoutCode;
	private RelativeLayout _adLayout = null;
	
	private boolean _bEnableADMenu = true;
	private boolean _bEnableADPlay = true;
	private boolean _bEnablePush = true;
	private boolean _bEnableAppDownload = true;
	private boolean _bADStrategyChanged = true;
	private boolean _bPushStrategyChanged = true;
	
	private String _channel_id = "";
	
	private String _bytesToHex(byte[] data)
	{
		if (data == null)
		{
			return null;
		}
		
		int len = data.length;
		String str = "";
		for (int i = 0; i < len; i++) 
		{
			String hs = java.lang.Integer.toHexString(data[i] & 0xFF);
			if ((data[i] & 0xFF) < 16)
				str = str + "0" + hs;
			else
				str = str + hs;
		}
		return str;
	}

	private byte[] _hexToBytes(String str)
	{
		if (str == null)
		{
			return null;
		}
		else if (str.length() < 2)
		{
			return null;
		}
		else
		{
			int i;
			int len = str.length() / 2;
			byte[] buffer = new byte[len];
			for (i = 0; i < len; i++)
			{
				buffer[i] = (byte) Integer.parseInt(
						str.substring(i * 2, i * 2 + 2), 16);
			}
			return buffer;
		}
	}
	
	private Crypt _c = new Crypt(
			_hexToBytes("37ca9a53c87eaa66f8fe88444f02a876"), 
			_hexToBytes("a3bb37149550b256009d23fc34b85836"));
	
	private String _idmogo = "38cc93f2f24666776bfd35924671a5691c6e1719857815d926e32ebc282c6f12";
	private String _idkuguo = "3279cf578b5ae4f064f3c80b5fbfc5ea8a5e0cd51d61f5c83e3570c9b79bbba3";

	private class Crypt
	{
		private IvParameterSpec ivspec;
		private SecretKeySpec keyspec;
		private Cipher cipher;

		//private String iv = "fedcba9876543210";//Dummy iv (CHANGE IT!)
		//private String SecretKey = "0123456789abcdef";//Dummy secretKey (CHANGE IT!)

		public Crypt(byte[] iv, byte[] sk)
		{
			ivspec = new IvParameterSpec(iv);
			keyspec = new SecretKeySpec(sk, "AES");

			try
			{
				cipher = Cipher.getInstance("AES/CBC/NoPadding");
			}
			catch (NoSuchAlgorithmException e)
			{
				e.printStackTrace();
				Logger.e(e);
			}
			catch (NoSuchPaddingException e)
			{
				e.printStackTrace();
				Logger.e(e);
			}
		}

		public String encryptString(String text)  throws Exception
		{
			byte[] r = encrypt(text);
			return _bytesToHex(r);
		}
		
		public String decryptString(String text) throws Exception
		{
			byte[] r = decrypt(text);
			return new String(r,"UTF-8");
		}
		
		public byte[] encrypt(String text) throws Exception
		{
			if (text == null || text.length() == 0) throw new Exception(
					"Empty string");

			byte[] encrypted = null;

			try
			{
				cipher.init(Cipher.ENCRYPT_MODE, keyspec, ivspec);
				encrypted = cipher.doFinal(padString(text).getBytes());
			}
			catch (Exception e)
			{
				Logger.e(e);
				throw new Exception("[encrypt] " + e.getMessage());
			}

			return encrypted;
		}

		public byte[] decrypt(String code) throws Exception
		{
			if (code == null || code.length() == 0) throw new Exception(
					"Empty string");

			byte[] decrypted = null;

			try
			{
				cipher.init(Cipher.DECRYPT_MODE, keyspec, ivspec);
				decrypted = cipher.doFinal(_hexToBytes(code));
			}
			catch (Exception e)
			{
				Logger.e(e);
				throw new Exception("[decrypt] " + e.getMessage());
			}
			return decrypted;
		}

		private String padString(String source)
		{
			char paddingChar = ' ';
			int size = 16;
			int align = (source.length() + (size - 1)) / size;
			align *= size;
			int padLength = align - source.length();

			for (int i = 0; i < padLength; i++)
			{
				source += paddingChar;
			}

			return source;
		}
	}

	private static final int _EVENT_Prepare = 0x0001;
	private static final int _EVENT_Play = 0x0002;

	private void _onGameEvent(int what)
	{
        switch (what)
        {
        case _EVENT_Prepare:
        	_updateADConfig();
     	   	_rebuildAD();
     	   	break;
        case _EVENT_Play:
        	if (!_bEnableADPlay)
        	{
        		_destroyAD();
        	}
     	   	break;
        }
	}
	
	private static class _ADManagerHandler extends Handler
	{
		WeakReference<MainActivity> mActivity;  
		  
		_ADManagerHandler(MainActivity activity) {  
		        mActivity = new WeakReference<MainActivity>(activity);  
		}  
		@Override
	    public void handleMessage(Message msg) {
           MainActivity theActivity = mActivity.get();
           theActivity._onGameEvent(msg.what);
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
				MobclickAgent.onEvent(MainActivity.this, val);
				if (val.equals("prepare"))
				{
					Message msg = new Message();
					msg.what = _EVENT_Prepare;
					_adManageHandler.sendMessage(msg);
				}
				if (val.equals("running"))
				{
					Message msg = new Message();
					msg.what = _EVENT_Play;
					_adManageHandler.sendMessage(msg);
				}
			}
		}
	};

	private UmengUpdateListener _updateListener = new UmengUpdateListener()
	{
		@Override
		public void onUpdateReturned(int updateStatus, UpdateResponse updateInfo)
		{
			switch (updateStatus)
			{
			case 0: // has update
				UmengUpdateAgent.showUpdateDialog(MainActivity.this, updateInfo);
				break;
			case 1: // has no update
				break;
			case 2: // none wifi
				break;
			case 3: // time out
				break;
			}

		}
	};

	private void _initUMeng()
	{
		try
		{
			ApplicationInfo appInfo = this.getPackageManager().getApplicationInfo(getPackageName(), PackageManager.GET_META_DATA);
			this._channel_id = appInfo.metaData.getString("UMENG_CHANNEL");
		}
		catch (Exception e)
		{
			Logger.e(e);
		}
		
		MobclickAgent.setSessionContinueMillis(60000 * 10);
		MobclickAgent.setDebugMode(false);
		MobclickAgent.onError(this);
		MobclickAgent.updateOnlineConfig(this);

		com.umeng.common.Log.LOG = false;

		UmengUpdateAgent.setUpdateOnlyWifi(false); // 目前我们默认在Wi-Fi接入情况下才进行自动提醒。如需要在其他网络环境下进行更新自动提醒，则请添加该行代码
		UmengUpdateAgent.setUpdateAutoPopup(false);
		UmengUpdateAgent.setUpdateListener(_updateListener);

		UmengUpdateAgent.setOnDownloadListener(new UmengDownloadListener()
		{
			@Override
			public void OnDownloadEnd(int result)
			{
				//"download result : " + result
			}

		});

		UmengUpdateAgent.update(this);
	}

	private void _updateADConfig()
	{
		boolean eam = true;
		boolean eap = true;
		boolean ep = true;
		boolean ed = true;
		try
		{
			//read configure
			String config = "";
			int retry = 2;
			while ((null == config || config.length() <= 0) && retry > 0)
			{
				config = MobclickAgent.getConfigParams(this, _channel_id);
				if (null == config || config.length() <= 0)
				{
					config = MobclickAgent.getConfigParams(this, "all");
				}
				retry--;
			}
			if (config.length() <= 0)
			{
				String c = Settings.get(this,  _channel_id);
				if (c.length() > 0)
				{
					config = _c.decryptString(c);
				}
				else
				{
					eam = true;
					eap = true;
					ep = false;
					ed = false;
				}
			}
			String c = _c.encryptString(config);
			Settings.set(this, _channel_id, c);
			
			String[] params = config.split(";");
			for (String param : params)
			{
				String[] items = param.split("=");
				if (items.length == 2)
				{
					if (items[0].equals("eam"))
					{
						eam = !items[1].equals("false");
					}
					else if (items[0].equals("eap"))
					{
						eap = !items[1].equals("false");
					}
					else if (items[0].equals("ep"))
					{
						ep = !items[1].equals("false");
					}
					else if (items[0].equals("ed"))
					{
						ed = !items[1].equals("false");
					}
				}
			}
		}
		catch (Exception e)
		{
			Logger.e(e);

			eam = true;
			eap = true;
			ep = false;
			ed = false;
		}
		
		String lang = GameEnvHandler.getLanguage();
		if (lang.equals("en"))
		{
			ep = false;
			ed = false;
		}
		
		if (eam != _bEnableADMenu)
		{
			_bEnableADMenu = eam;
			_bADStrategyChanged = true;
		}
		if (eap != _bEnableADPlay)
		{
			_bEnableADPlay = eap;
			_bADStrategyChanged = true;
		}
		if (ep != _bEnablePush)
		{
			_bEnablePush = ep;
			_bPushStrategyChanged = true;
		}
		if (ed != _bEnableAppDownload)
		{
			_bEnableAppDownload = ed;
			_bPushStrategyChanged = true;
		}
		String log = String.format("eam=%s, eap=%s, ep=%s, ed=%s", 
				Utils.boolean2str(eam),
				Utils.boolean2str(eap),
				Utils.boolean2str(ep),
				Utils.boolean2str(ed)
				);
		Log.d(TAG, log);
	}
	
	private void _rebuildAD()
	{
		try
		{
			if (_bADStrategyChanged)
			{
				_bADStrategyChanged = false;
				if (null != _adsMogoLayoutCode)
				{
					_destroyAD();
				}
			}
	        
			if (null != _adsMogoLayoutCode)
				return;
			
			// ���췽�������ÿ���ģʽ
			_adsMogoLayoutCode = new AdsMogoLayout(this, _idmogo, false);
	
			// ���ü���ص� ���а��� ���� չʾ ����ʧ�ܵ��¼��Ļص�
			_adsMogoLayoutCode.setAdsMogoListener(this);
	
			/*------------------------------------------------------------*/
			// ͨ��Code��ʽ��ӹ���� ����Ľṹ����(�����ο�)
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
			RelativeLayout.LayoutParams layoutParams;
			layoutParams = new RelativeLayout.LayoutParams(
					RelativeLayout.LayoutParams.FILL_PARENT,
					RelativeLayout.LayoutParams.WRAP_CONTENT);
			layoutParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM,
					RelativeLayout.TRUE);
	
			RelativeLayout.LayoutParams layoutParamsAD;
			layoutParamsAD = new RelativeLayout.LayoutParams(
					RelativeLayout.LayoutParams.WRAP_CONTENT,
					RelativeLayout.LayoutParams.WRAP_CONTENT);
			layoutParamsAD.addRule(RelativeLayout.ALIGN_PARENT_LEFT,
					RelativeLayout.TRUE);
	
			RelativeLayout childLayout = new RelativeLayout(this);
			childLayout.addView(_adsMogoLayoutCode, layoutParamsAD);
	
			_adLayout.removeAllViews();
			_adLayout.addView(childLayout, layoutParams);
		}
		catch (Exception e)
		{
			Logger.e(e);
		}
	}
	
	private void _destroyAD()
	{
		try
		{
			if (null != _adsMogoLayoutCode)
			{
				_adsMogoLayoutCode.clearThread();
				_adsMogoLayoutCode = null;
			}
			_adLayout.removeAllViews();
		}
		catch (Exception e)
		{
			Logger.e(e);
		}
	}

	private void _rebuildPush()
	{
		if (!_bPushStrategyChanged)
			return;
		_bPushStrategyChanged = false;
		
		KuguoAdsManager.getInstance().setCooId(this, _idkuguo);
		if (_bEnableAppDownload)
		{
			KuguoAdsManager.getInstance().showKuguoSprite(this,
					KuguoAdsManager.STYLE_KUZAI);
			KuguoAdsManager.getInstance().setKuzaiPosition(true, 0);
		}

		if (_bEnablePush)
		{
			KuguoAdsManager.getInstance().receivePushMessage(
					MainActivity.this, false);
			MobclickAgent.onEvent(MainActivity.this, "push");
		}
	}
	/**
	 * ���û�������*(����Mogo�����¼�����)
	 */
	@Override
	public void onClickAd(String arg0)
	{
		Log.d(AdsMogoUtil.ADMOGO, "-=onClickAd=-");
	}

	/**
	 * ���û�����˹��رհ�ťʱ�ص�(�رչ�水ť���ܿ�����Mogo��App����������)
	 */
	@Override
	public void onCloseAd()
	{
		Log.d(AdsMogoUtil.ADMOGO, "-=onCloseAd=-");
	}

	/**
	 * ���û��ر����������͹�����ϸ����ʱ�ص�(�����������Ϊ���ع�沢���ǵ���������صĲŻ��д�
	 * Dialog)
	 */
	@Override
	public void onCloseMogoDialog()
	{
		Log.d(AdsMogoUtil.ADMOGO, "-=onCloseMogoDialog=-");
	}

	/**
	 * ���й��ƽ̨����ʧ��ʱ�ص�
	 */
	@Override
	public void onFailedReceiveAd()
	{
		Log.d(AdsMogoUtil.ADMOGO, "-=onFailedReceiveAd=-");
	}

	/**
	 * ���û�������*(��ʵ��� Mogo��һ���ڵ��ô˻ص�ʱ��¼�����)
	 */
	@Override
	public void onRealClickAd()
	{
		Log.d(AdsMogoUtil.ADMOGO, "-=onRealClickAd=-");
	}

	/**
	 * ������ɹ�ʱ�ص�
	 */
	@Override
	public void onReceiveAd(ViewGroup arg0, String arg1)
	{
		Log.d(AdsMogoUtil.ADMOGO, "-=onReceiveAd=-");
	}

	/**
	 * ��ʼ������ʱ�ص�
	 */
	@Override
	public void onRequestAd(String arg0)
	{
		Log.d(AdsMogoUtil.ADMOGO, "-=onRequestAd=-");
	}

	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		//String v = Utils.getPackageVersion(this);
		
		Logger.start(this,  Logger.WARN, Logger.DEBUG);
		Logger.d(TAG, "craete");
		
		Utils.killUnrelatedActivityProcesses(this);
		GameEnvHandler.setListener(this, _eventListener);
		
		//encrypt mogo
//		try
//		{
//			String enmogo_cn = _c.encryptString("c87eaafe88444f02a866f87637ca9a53");
//			String demogo_cn = _c.decryptString(enmogo_cn);
//			String enmogo_en = _c.encryptString("17b388b8b05841899761b326f2f4fa86");
//			String demogo_en = _c.decryptString(enmogo_en);
//			String enkugo = _c.encryptString("0b25685800144b9da3bb373695523fc3");
//			String dekugo = _c.decryptString(enkugo);
//		 
//			enkugo = dekugo;
//		}
//		catch (Exception e)
//		{
//			String em = e.getMessage();
//		}
		
		try
		{
			//("c87eaafe88444f02a866f87637ca9a53");
			//("17b388b8b05841899761b326f2f4fa86");
			//("0b25685800144b9da3bb373695523fc3");
			String lang = GameEnvHandler.getLanguage();
			String idmogo = lang.equals("en") ? 
					"5889a8f0c5ee63e458382b6f00a53c2be3b91ee676adb4afae1bc7604e235d30" : 
					"24671a5691c6e17198538cc93f2f226e32ebc282c6f124666776bfd3597815d9";
			_idmogo = _c.decryptString(idmogo);
			_idkuguo = _c.decryptString("bba364f3c80b5fbfc53279cd51d61f5c83e3570c9b79bcf578b5ae4f0e0ea8a5");
		}
		catch (Exception e)
		{
			//String em = e.getMessage();
			Logger.e(e);
		}
		
		try
		{
			if (detectOpenGLES20())
			{
				// get the packageName,it's used to set the resource path
				String packageName = getApplication().getPackageName();
				super.setPackageName(packageName);
	
				// FrameLayout
				ViewGroup.LayoutParams framelayout_params = new ViewGroup.LayoutParams(
						ViewGroup.LayoutParams.FILL_PARENT,
						ViewGroup.LayoutParams.FILL_PARENT);
				FrameLayout framelayout = new FrameLayout(this);
				framelayout.setLayoutParams(framelayout_params);
	
				// Cocos2dxEditText layout
				ViewGroup.LayoutParams edittext_layout_params = new ViewGroup.LayoutParams(
						ViewGroup.LayoutParams.FILL_PARENT,
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
						RelativeLayout.LayoutParams.FILL_PARENT,
						RelativeLayout.LayoutParams.FILL_PARENT));
				
				_initUMeng();
				_updateADConfig();
				
				_rebuildPush();
			}
			else
			{
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
		try
		{
			super.onPause();
			MobclickAgent.onPause(this);
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
		try
		{
			super.onResume();
			MobclickAgent.onResume(this);
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
		try
		{
			_destroyAD();
			super.onDestroy();
			//回收接口，退出酷仔及回收酷仔资源
			KuguoAdsManager.getInstance().recycle(this);
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
