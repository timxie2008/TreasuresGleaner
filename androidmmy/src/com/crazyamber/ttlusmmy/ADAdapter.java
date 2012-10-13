package com.crazyamber.ttlusmmy;

import java.util.List;

import com.crazyamber.core.*;

import com.mobile.app.main.GEInstance;
import com.mobile.app.main.GEListener;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

public class ADAdapter implements GEListener
{
	private static final String	TAG	= "ADA";

	public static final int EVENT_Loaded = 0x0001;
	public static final int EVENT_Prepare = 0x0002;
	public static final int EVENT_Play = 0x0003;

    private GEInstance geInstance;
    
    private boolean _adLoaded = false;
	private RelativeLayout _adLayout;

	private Activity _activity = null;
	private Context _context = null;
	
	private AnalyzerAdapter _analyzer = null;

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
				if (_adLoaded)
				{
					Log.d(TAG, "ar");
					_destroyAD();
				}
			}
	        
			if (_adLoaded)
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
	
			LinearLayout ll = new LinearLayout(_context); 
			RelativeLayout childLayout = new RelativeLayout(_context);
			childLayout.addView(ll, layoutParamsAD);
	        //互动广告
	        //LinearLayout interLinearLayout=(LinearLayout)findViewById(R.id.interGELinearLayout);//初始化互动广告必须的布局
	        geInstance.loadInterAd(5, GEInstance.INTERSPACEY, ll);//加载互动广告
	        geInstance.setInterAdVisible(View.VISIBLE);//显示或隐藏互动广告

			_adLayout.removeAllViews();
			_adLayout.addView(childLayout, layoutParams);
			
			_adLoaded = true;
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
			_adLayout.removeAllViews();
			_adLoaded = false;
		}
		catch (Exception e)
		{
			Logger.e(e);
		}
	}

	private void _rebuildPush()
	{
        //推送广告
        geInstance.loadPushAd();//加载推送广告(1.与上次推送调用时间间隔满足两小时才会推送  2.如果满足时间间隔条件,但是是相同广告也会取消推送)
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
		
        geInstance = new GEInstance();
        
        //初始函数
        geInstance.initialize(_context,"10000","T1001");//每次程序启动必须要初始化一次(设置开发者UID和PID)
        
        geInstance.setTestMode(true);//开启测试模式(默认是关闭的,测试的时候可以开启,方便调试并查看广告后台错误信息)
        geInstance.setOnGEListener(this);//继承GEListener接口(1.监听自定义广告数据  2.监听是否获取金币成功)
        geInstance.setNotificationIcon(R.drawable.icon);//设置状态栏图标
        geInstance.openListAdOn(false);//是否开启点击任意(自定义广告除外)广告都打开积分墙(默认不开启)
        geInstance.getGEVersion();//获取当前广告客户端的版本
        //虚拟积分
        geInstance.setSocreUnit("金蛋");//设置积分的单位(例如:金币 金蛋 人民币等)
        geInstance.setScoreParam(1);//设置金币的转换率(例如:如果默认下载一款软件得到的积分是50,若设置转换率为10,则最终显示并赋予用户50*10=500金币)
        geInstance.setDefalutScore(300);//设置给用户的默认积分(默认为0)
        //自定义广告
        //geInstance.loadInfoAd(1,true,true);//加载迷你广告(参数1:加载广告的数量(建议1~10)  参数2:是否下载广告的截图(单项自定义广告建议下载)  参数3:是否使用内置界面显示广告,如果用自定义布局则不需要开启此选项)
        //geInstance.loadInfoAd(5,false,true);
        //迷你广告
        //LinearLayout miniLinearLayout=(LinearLayout)findViewById(R.id.miniGELinearLayout);//初始化迷你广告必须的布局
        //geInstance.loadMiniAd(5,GEInstance.MINIFADE, _adLayout);//加载迷你广告(所有广告在加载完毕后会自动显示)
        //geInstance.setMiniBackColor(0xffff0000);//设置迷你广告背景颜色
        //geInstance.setMiniTextColor(0xff00ff00);//设置迷你广告文字颜色
        //geInstance.setMiniAdVisible(View.VISIBLE);//显示或隐藏迷你广告
        
		_updateADConfig();
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
        	_rebuildPush();
        	break;
        case ADAdapter.EVENT_Prepare:
        	_updateADConfig();
     	   	_rebuildAD();
	        //列表广告
	        GEInstance.setListName("积分墙");//修改积分墙标题名称
	        GEInstance.loadListAd();//加载列表广告
     	   	break;
        case ADAdapter.EVENT_Play:
        	if (!_config.bEnableADPlay)
        	{
        		_destroyAD();
        	}
     	   	break;
        }
	}

	@Override
	public void onAdFailed(String arg0)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onAdSucceed(int arg0)
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onInfoList(List arg0)
	{
		// TODO Auto-generated method stub
		
	}
}
