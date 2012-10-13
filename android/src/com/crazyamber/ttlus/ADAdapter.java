package com.crazyamber.ttlus;

import com.crazyamber.core.*;

import com.adsmogo.adview.AdsMogoLayout;
import com.adsmogo.controller.listener.AdsMogoListener;
import com.kuguo.ad.KuguoAdsManager;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.RelativeLayout;

public class ADAdapter implements AdsMogoListener
{
	private static final String	TAG	= "ADA";

	public static final int EVENT_Loaded = 0x0001;
	public static final int EVENT_Prepare = 0x0002;
	public static final int EVENT_Play = 0x0003;

	private AdsMogoLayout _adsMogoLayoutCode;
	private RelativeLayout _adLayout;

	Activity _activity = null;
	Context _context = null;
	
	AnalyzerAdapter _analyzer = null;

	private SimpleCipher _c = new SimpleCipher(
			Utils.hexToBytes("37ca9a53c87eaa66f8fe88444f02a876"), 
			Utils.hexToBytes("a3bb37149550b256009d23fc34b85836"));
	
	public ADAdapter()
	{
		_context = null;
	}
	
	private class _ADConfig
	{
		private String channel = "";
		private String idcn = "";
		private String iden = "";
		
		private int nClickLimit = 0x7fffffff;
		private boolean bEnableADMenu = false;
		private boolean bEnableADPlay = false;
		private boolean bEnablePush = false;
		private boolean bEnableAppDownload = false;
		
		private boolean bDirty = false;
		
		private int _hashCode()
		{
			int n = 0;
			n ^= channel.hashCode();
			n ^= idcn.hashCode();
			n ^= iden.hashCode();
			n ^= nClickLimit;
			n ^= bEnableADMenu ? 0xabcdef : 0x23deba;
			n ^= bEnableADPlay ? 0x7ba8fa : 0x456792;
			n ^= bEnablePush ? 0x35dea6 : 0xdeacb6;
			n ^= bEnableAppDownload ? 0x45fba7 : 0x235dae;
			return n;
		}
		private String adid()
		{
			String lang = GameEnvHandler.getLanguage();
			if (lang.equals("en"))
				return iden;
			return idcn;
		}
		public void setDirty(boolean b) { bDirty = b; }
		public boolean isDirty()
		{
			return bDirty;
		}
		
		public boolean parse(String ch, String config)
		{
			if (null == config || config.length() <= 0)
				return false;
			
			String[] params = config.split(";");
			if (params.length <= 0)
				return false;
			
			int hc = _hashCode();
			if (!channel.equals(ch))
			{
				channel = ch;
			}
			for (String param : params)
			{
				String[] items = param.split("=");
				if (items.length == 2)
				{
					if (items[0].equals("iden"))
					{
						if (!iden.equals(items[1]))
						{
							iden = items[1];
						}
					}
					if (items[0].equals("idcn"))
					{
						if (!idcn.equals(items[1]))
						{
							idcn = items[1];
						}
					}
					if (items[0].equals("eam"))
					{
						boolean b = !items[1].equals("false");
						if (b != bEnableADMenu)
						{
							bEnableADMenu = b;
						}
					}
					else if (items[0].equals("eap"))
					{
						boolean b = !items[1].equals("false");
						if (b != bEnableADPlay)
						{
							bEnableADPlay = b;
						}
					}
					else if (items[0].equals("ep"))
					{
						boolean b = !items[1].equals("false");
						if (b != bEnablePush)
						{
							bEnablePush = b;
						}
					}
					else if (items[0].equals("ed"))
					{
						boolean b = !items[1].equals("false");
						if (b != bEnableAppDownload)
						{
							bEnableAppDownload = b;
						}
					}
				}
			}
			int hcn = _hashCode();
			bDirty = hc != hcn;

			return true;
		}
	};
	
	private String _channel_id = "";
	private _ADConfig _config = new _ADConfig();

	private void _rebuildAD()
	{
		try
		{
			if (_config.isDirty())
			{
				_config.setDirty(false);
				if (null != _adsMogoLayoutCode)
				{
					Log.d(TAG, "ar");
					_destroyAD();
				}
			}
	        
			if (null != _adsMogoLayoutCode)
			{
				Log.d(TAG, "a1");
				return;
			}
			
			Log.d(TAG, "a0");
			String adid = "";
			try
			{
				adid = _c.decryptString(_config.adid());
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}
			if (adid.length() <= 0)
			{
				Log.d(TAG, "ae");
				return;
			}

			Log.d(TAG, "ab");
			_adsMogoLayoutCode = new AdsMogoLayout(_activity, adid, false);
	
			_adsMogoLayoutCode.setAdsMogoListener(this);
	
			RelativeLayout.LayoutParams layoutParams;
			layoutParams = new RelativeLayout.LayoutParams(
					RelativeLayout.LayoutParams.MATCH_PARENT,
					RelativeLayout.LayoutParams.WRAP_CONTENT);
			layoutParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM,
					RelativeLayout.TRUE);
	
			RelativeLayout.LayoutParams layoutParamsAD;
			layoutParamsAD = new RelativeLayout.LayoutParams(
					RelativeLayout.LayoutParams.WRAP_CONTENT,
					RelativeLayout.LayoutParams.WRAP_CONTENT);
			layoutParamsAD.addRule(RelativeLayout.ALIGN_PARENT_LEFT,
					RelativeLayout.TRUE);
	
			RelativeLayout childLayout = new RelativeLayout(_context);
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
		if (_config.bEnablePush || _config.bEnableAppDownload)
		{
		}
		else
		{
			return;
		}
		
		String idkuguo = "";
		try
		{
			idkuguo = _c.decryptString("bba364f3c80b5fbfc53279cd51d61f5c83e3570c9b79bcf578b5ae4f0e0ea8a5");
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		if (idkuguo.length() <= 0)
			return;
		
		KuguoAdsManager.getInstance().setCooId(_context, idkuguo);
		if (_config.bEnableAppDownload)
		{
			KuguoAdsManager.getInstance().showKuguoSprite(_context,
					KuguoAdsManager.STYLE_KUZAI);
			KuguoAdsManager.getInstance().setKuzaiPosition(true, 0);
		}

		if (_config.bEnablePush)
		{
			KuguoAdsManager.getInstance().receivePushMessage(
					_context, false);
			_analyzer.onEvent(_context, "push");
		}
	}

	@Override
	public void onClickAd(String arg0)
	{
		//Log.d(AdsMogoUtil.ADMOGO, "-=onClickAd=-");
	}

	@Override
	public void onCloseAd()
	{
		//Log.d(AdsMogoUtil.ADMOGO, "-=onCloseAd=-");
	}

	@Override
	public void onCloseMogoDialog()
	{
		//Log.d(AdsMogoUtil.ADMOGO, "-=onCloseMogoDialog=-");
	}

	@Override
	public void onFailedReceiveAd()
	{
		//Log.d(AdsMogoUtil.ADMOGO, "-=onFailedReceiveAd=-");
	}

	@Override
	public void onRealClickAd()
	{
		//Log.d(AdsMogoUtil.ADMOGO, "-=onRealClickAd=-");
	}

	@Override
	public void onReceiveAd(ViewGroup arg0, String arg1)
	{
		//Log.d(AdsMogoUtil.ADMOGO, "-=onReceiveAd=-");
	}

	@Override
	public void onRequestAd(String arg0)
	{
		//Log.d(AdsMogoUtil.ADMOGO, "-=onRequestAd=-");
	}
	
	private void _updateADConfig()
	{
//		eam=true;eap=true;ep=false;ed=false;iden=5889a8f0c5ee63e458382b6f00a53c2be3b91ee676adb4afae1bc7604e235d30;idcn=24671a5691c6e17198538cc93f2f226e32ebc282c6f124666776bfd3597815d9
//		new String ("all;cn;24671a5691c6e17198538cc93f2f226e32ebc282c6f124666776bfd3597815d9"),
//		new String ("all;en;5889a8f0c5ee63e458382b6f00a53c2be3b91ee676adb4afae1bc7604e235d30"),
//		new String ("douding;cn;24671a5691c6e17198538cc93f2f226e32ebc282c6f124666776bfd3597815d9"),
//		new String ("douding;en;5889a8f0c5ee63e458382b6f00a53c2be3b91ee676adb4afae1bc7604e235d30"),
		String config;
		String[] channels = { _channel_id, "all" };
		
		boolean hit = false;
		for (String channel : channels)
		{
			config = _analyzer.getConfigParams(channel);
			if (_config.parse(channel, config))
			{
				try
				{
					config = _c.encryptString(config);
					Settings.set(_context, channel, config);
				}
				catch (Exception e)
				{
					e.printStackTrace();
				}
				hit = true;
				break;
			}
		}
		
		if (!hit)
		{
			//fill default params
			_config.bEnableADMenu = true;
			_config.bEnableADPlay = true;
			_config.bEnablePush = false;
			_config.bEnableAppDownload = false;
			_config.channel = _channel_id;
			_config.iden = "5889a8f0c5ee63e458382b6f00a53c2be3b91ee676adb4afae1bc7604e235d30";
			_config.idcn = "24671a5691c6e17198538cc93f2f226e32ebc282c6f124666776bfd3597815d9";
			_config.setDirty(true);
			
			for (String channel : channels)
			{
				String rc = Settings.get(_context,  channel);
				try
				{
					config = _c.decryptString(rc);
				}
				catch (Exception e)
				{
					config = "";
				}
				
				if (_config.parse(channel, config))
				{
					break;
				}
			}
		}
		Log.d(TAG, "uac");
	}
	
	public void setup(Activity act, RelativeLayout adLayout, AnalyzerAdapter analyzer)
	{
		_activity = act;
		_context = act;
		_adLayout = adLayout;
		_analyzer = analyzer;
		try
		{
			ApplicationInfo appInfo = 
					_context.getPackageManager().getApplicationInfo(
							_context.getPackageName(), PackageManager.GET_META_DATA);
			this._channel_id = appInfo.metaData.getString("UMENG_CHANNEL");
		}
		catch (Exception e)
		{
			Logger.e(e);
		}
		_updateADConfig();
	}
	
	public void cleanup()
	{
		_destroyAD();
		KuguoAdsManager.getInstance().recycle(_context);
	}
	
	public void update(int what)
	{
        switch (what)
        {
        case ADAdapter.EVENT_Loaded:
        	_rebuildPush();
        	break;
        case ADAdapter.EVENT_Prepare:
        	_updateADConfig();
     	   	_rebuildAD();
     	   	break;
        case ADAdapter.EVENT_Play:
        	if (!_config.bEnableADPlay)
        	{
        		_destroyAD();
        	}
     	   	break;
        }
	}
}
