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
import com.kuguo.ad.KuguoAdsManager;
import com.umeng.analytics.MobclickAgent;
import com.umeng.update.UmengDownloadListener;
import com.umeng.update.UmengUpdateAgent;
import com.umeng.update.UmengUpdateListener;
import com.umeng.update.UpdateResponse;

import android.widget.RelativeLayout;

import java.security.NoSuchAlgorithmException;

import javax.crypto.Cipher;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class MainActivity extends Cocos2dxActivity implements AdsMogoListener
{

	private Cocos2dxGLSurfaceView mGLView;
	private AdsMogoLayout adsMogoLayoutCode;
	private boolean _bEnableAD = true;
	private boolean _bEnablePush = true;
	private boolean _bEnableAppDownload = true;
	
	private String _bytesToHex(byte[] data)
	{
		if (data==null)
		{
			return null;
		}
		
		int len = data.length;
		String str = "";
		for (int i=0; i < len; i++) 
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
			int len = str.length() / 2;
			byte[] buffer = new byte[len];
			for (int i = 0; i < len; i++)
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
	
	private String _idmogo = "24671a5691c6e17198538cc93f2f226e32ebc282c6f124666776bfd3597815d9";
	private String _idkuguo = "bba364f3c80b5fbfc53279cd51d61f5c83e3570c9b79bcf578b5ae4f0e0ea8a5";

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
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			catch (NoSuchPaddingException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
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
				throw new Exception("[decrypt] " + e.getMessage());
			}
			return decrypted;
		}

		private String padString(String source)
		{
			char paddingChar = ' ';
			int size = 16;
			int align = source.length() + (size - 1) / size;
			int padLength = align - source.length();

			for (int i = 0; i < padLength; i++)
			{
				source += paddingChar;
			}

			return source;
		}
	}

	private GameEventHandler.GameEventListener _eventListener = new GameEventHandler.GameEventListener()
	{
		@Override
		public void onEvent(String evt, String val)
		{
			if (evt.equals("play_state"))
			{
				MobclickAgent.onEvent(MainActivity.this, val);
				if (val.equals("running"))
				{
					if (_bEnablePush)
					{
						KuguoAdsManager.getInstance().receivePushMessage(
								MainActivity.this, true);
					}
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
				UmengUpdateAgent
						.showUpdateDialog(MainActivity.this, updateInfo);
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

		String ea = MobclickAgent.getConfigParams(this, "ea");
		_bEnableAD = ea.equals("true");
		String strEnablePush = MobclickAgent.getConfigParams(this, "ep");
		_bEnablePush = strEnablePush.equals("true");
		String strEnableAppDownload = MobclickAgent.getConfigParams(this, "ed");
		_bEnableAppDownload = strEnableAppDownload.equals("true");
	}

	private void _initAD()
	{
		/*------------------------------------------------------------*/
		// ��ʼ��AdsMogoLayout ��ʼ����Ϊ���¼��ַ�ʽ
		// ���췽�������ù�����ͣ���ȫ����棬banner���
		// public AdsMogoLayout(final Activity context, final String keyAdMogo,
		// final int ad_type) {
		// }

		// Ĭ�ϵĹ��췽����Ĭ�Ͽ�������ģʽ��banner���
		// public AdsMogoLayout(final Activity context, final String keyAdMogo)
		// {
		// }

		// ���췽�������ÿ���ģʽ
		// public AdsMogoLayout(final Activity context, final String keyAdMogo,
		// boolean expressMode) {
		// }

		// ���췽�������ù�����ͺͿ���ģʽ
		// public AdsMogoLayout(final Activity context, final String keyAdMogo,
		// final int ad_type, final boolean expressMode) {
		// }
		/*------------------------------------------------------------*/

		if (!_bEnableAD) return;
		
		// ���췽�������ÿ���ģʽ
		adsMogoLayoutCode = new AdsMogoLayout(this, _idmogo, false);

		// ���ü���ص� ���а��� ���� չʾ ����ʧ�ܵ��¼��Ļص�
		adsMogoLayoutCode.setAdsMogoListener(this);

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
		childLayout.addView(adsMogoLayoutCode, layoutParamsAD);

		RelativeLayout parentLayput = new RelativeLayout(this);
		parentLayput.addView(childLayout, layoutParams);

		this.addContentView(parentLayput, new RelativeLayout.LayoutParams(
				RelativeLayout.LayoutParams.FILL_PARENT,
				RelativeLayout.LayoutParams.FILL_PARENT));
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

		//encrypt mogo
//		try
//		{
//			String enmogo = _c.encryptString("c87eaafe88444f02a866f87637ca9a53");
//			String demogo = _c.decryptString(enmogo);
//			String enkugo = _c.encryptString("0b25685800144b9da3bb373695523fc3");
//			String dekugo = _c.decryptString(enkugo);
//			enmogo = demogo;
//			enkugo = dekugo;
//		}
//		catch (Exception e)
//		{
//			String em = e.getMessage();
//		}
		
		try
		{
			//("c87eaafe88444f02a866f87637ca9a53");
			//("0b25685800144b9da3bb373695523fc3");
			_idmogo = _c.decryptString(_idmogo);
			_idkuguo = _c.decryptString(_idkuguo);
		}
		catch (Exception e)
		{
			String em = e.getMessage();
		}
		
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

			_initUMeng();
			_initAD();

			GameEventHandler.setListener(_eventListener);

			KuguoAdsManager.getInstance().setCooId(this, _idkuguo);
			if (_bEnableAppDownload)
			{
				KuguoAdsManager.getInstance().showKuguoSprite(this,
						KuguoAdsManager.STYLE_KUZAI);
				KuguoAdsManager.getInstance().setKuzaiPosition(true, -20);
			}
		}
		else
		{
			Log.d("activity", "don't support gles2.0");
			finish();
		}
	}

	@Override
	protected void onPause()
	{
		super.onPause();
		MobclickAgent.onPause(this);
		mGLView.onPause();
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		MobclickAgent.onResume(this);
		mGLView.onResume();
	}

	@Override
	protected void onDestroy()
	{
		// ��� adsMogoLayout ʵ�� ��������ڶ��̻߳�����Ƶ��̳߳�
		if (adsMogoLayoutCode != null)
		{
			adsMogoLayoutCode.clearThread();
		}
		super.onDestroy();
		//回收接口，退出酷仔及回收酷仔资源
		KuguoAdsManager.getInstance().recycle(this);
	}

	private boolean detectOpenGLES20()
	{
		ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		ConfigurationInfo info = am.getDeviceConfigurationInfo();
		return (info.reqGlEsVersion >= 0x20000);
	}

	static
	{
		System.loadLibrary("game_frame");
	}
}
