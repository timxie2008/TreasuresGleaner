package com.crazyamber.haidip;

import com.crazyamber.core.*;

import com.adsmogo.adview.AdsMogoLayout;
import com.adsmogo.controller.listener.AdsMogoListener;
//import com.kuguo.ad.KuguoAdsManager;

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

	private String _idmogoen = "10e516b1f258e4241deb216812e1cbecf7a3ad38e57a531ecf623f777d960f2a0e7bf4f89a5fd4c38ac52b8cbe52da2a";
	private String _idmogocn = "10e516b1f258e4241deb216812e1cbecf7a3ad38e57a531ecf623f777d960f2a0e7bf4f89a5fd4c38ac52b8cbe52da2a";
	//eam=true;eap=true;ep=false;ed=false;iden=fb50f5d51d6e40d93417ecaae48c23011222f9e044abe203ef0d9ef6eeefa2a19c201bc8d26b5b6d9fb02fa624a1d322;idcn=3a6d3b0eedd952f93fe811ea91fd5b53581c5a123baa03d27c1e680bfdb38e99fae86d449b50ef87b7277721d9544c7d
	
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
		
		public boolean parse(Context c, String ch, String config)
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
					else if (items[0].equals("idcn"))
					{
						if (!idcn.equals(items[1]))
						{
							idcn = items[1];
						}
					}
					else if (items[0].equals("eam"))
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
					else
					{
						Settings.set(c, items[0], items[1]);
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

	private SimpleCipher _c = new SimpleCipher(
			Utils.hexToBytes("37ca9a53c87eaa66f8fe88444f02a876"), 
			Utils.hexToBytes("a3bb37149550b256009d23fc34b85836"));
	
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
			Logger.e(TAG, e);
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
			Logger.e(TAG, e);
		}
	}

	private void _rebuildPush()
	{
		/*
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
			idkuguo = _c.decryptString(_idkuguo);
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
		*/
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
		String config;
		String v = Utils.getPackageVersion(_context);
		String[] channels = { _channel_id + "_" + v, _channel_id, "default_" + v, "default"};
		
		boolean hit = false;
		for (String channel : channels)
		{
			config = _analyzer.getConfigParams(channel);
			if (_config.parse(_context, channel, config))
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
			_config.iden = _idmogoen;
			_config.idcn = _idmogocn;
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
				
				if (_config.parse(_context, channel, config))
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
			_channel_id = appInfo.metaData.getString("UMENG_CHANNEL");
			if (null == _channel_id)
			{
				_channel_id = "";
			}
			//set default params first
			Settings.setInt(_context, "pmaxcount", 16);
			Settings.setInt(_context, "pminmin", 5);
			Settings.setInt(_context, "hmin", 1);
			Settings.setInt(_context, "hmax", 23);
			_updateADConfig();
		}
		catch (Exception e)
		{
			Logger.e(TAG, e);
		}
	}
	
	public void cleanup()
	{
		_destroyAD();
	}
	
	public void update(int what)
	{
        switch (what)
        {
        case ADAdapter.EVENT_Loaded:
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
