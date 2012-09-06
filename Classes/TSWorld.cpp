#include "TSWorld.h"

#include "TSStageTest.h"
#	include "TSStageTestLayerTest.h"
#		include "TSStageTestLayerTestBubbles.h"

#include "TSStageLogo.h"
#	include "TSStageLogoLayerMain.h"

//#include "TSStageMenu.h"
//#	include "TSStageMenuLayerMain.h"

#include "TSStagePlay.h"
#	include "TSStagePlayLayerGame.h"
#	include "TSStagePlayLayerGamePrepare.h"
#	include "TSStagePlayLayerGameShop.h"
#	include "TSStagePlayLayerGamePlay.h"
#	include "TSStagePlayLayerGamePlayPause.h"
//#		include "TSStagePlayLayerGameBackground.h"
//#		include "TSStagePlayLayerGameDoodads1.h"
//#		include "TSStagePlayLayerGamePlay.h"
//#			include "TSStagePlayLayerGamePlayDialog.h"
//#		include "TSStagePlayLayerGameDoodads2.h"

//#include "TSStageHelp.h"
//#	include "TSStageHelpLayerMain.h"

//#include "TSStageOptions.h"
//#	include "TSStageOptionsLayerMain.h"

//#include "TSStageCredits.h"
//#	include "TSStageCreditsLayerMain.h"

//lay on the road
#	include "TSStagePlayLayerCommon.h"

#include "TSSpriteFish.h"
#include "TSSpritePlayer.h"
#include "TSSpriteBubble.h"
#include "TSSpriteCommon.h"
#include "TSSpriteButton.h"

TSWorld::TSWorld(void)
{
}

TSWorld::~TSWorld(void)
{
}

static const char* _stage_play			= "game/game";
static		const char* _layer_game		= "game";
static			const char* _layer_game_prepare	= "game.prepare";
static			const char* _layer_game_shop	= "game.shop";
static			const char* _layer_game_play	= "game.play";
static			const char* _layer_game_play_pause	= "game.play.pause";

static const char* _stage_test			= "test";
static		const char* _layer_test		= "test";
static			const char* _layer_test_bubbles = "test.bubbles";

static const char* _stage_logo			= "logo/logo";
static		const char* _layer_logo		= "logo";

//static const char* _stage_menu			= "menu";
//static		const char* _layer_menu		= "menu";

//static const char* _stage_help			= "help";
//static		const char* _layer_help		= "help";

//static const char* _stage_options		= "options";
//static		const char* _layer_options	= "options";

//static const char* _stage_credits		= "credits";
//static		const char* _layer_credits	= "credits";

CAStage* TSWorld::createStage(const char* _name)
{
	GUARD_FUNCTION();

	string name = _name;
	CAStage* ps = NULL;
	if (name == _stage_play)
	{
		GUARD_FIELD(stage_play);
		ps = new TSStagePlay(_name);
	}
	else if (name == _stage_logo)
	{
		GUARD_FIELD(stage_logo);
		ps = new TSStageLogo(_name);
	}
	/*
	else if (name == _stage_menu)
	{
		GUARD_FIELD(stage_menu);
		ps = new TSStageMenu(_name);
	}
	else if (name == _stage_help)
	{
		GUARD_FIELD(stage_help);
		ps = new TSStageHelp(_name);
	}
	else if (name == _stage_options)
	{
		GUARD_FIELD(stage_options);
		ps = new TSStageOptions(_name);
	}
	else if (name == _stage_credits)
	{
		GUARD_FIELD(stage_credits);
		ps = new TSStageCredits(_name);
	}
	*/
	else if (name == _stage_test)
	{
		GUARD_FIELD(stage_test);
		ps = new TSStageTest(_name);
	}

	return ps;
}

CAStageLayer* TSWorld::createLayer(CAStage* pstage, CAStageLayer* playerParent, const char* _name)
{
	GUARD_FUNCTION();

	string name = "";
	
	if (playerParent)
	{
	//	name += playerParent->name();
	}
	name += _name;

	if (pstage->name() == _stage_play)
	{
		if (name == _layer_game)
		{
			GUARD_FIELD(layer_game);
			return new TSStagePlayLayerGame(pstage, playerParent);
		}
		else if (name == _layer_game_prepare)
		{
			GUARD_FIELD(layer_game_prepare);
			return new TSStagePlayLayerGamePrepare(pstage, playerParent);
		}
		else if (name == _layer_game_play)
		{
			GUARD_FIELD(layer_game_play);
			return new TSStagePlayLayerGamePlay(pstage, playerParent);
		}
		else if (name == _layer_game_shop)
		{
			GUARD_FIELD(layer_game_shop);
			return new TSStagePlayLayerGameShop(pstage, playerParent);
		}
		else if (name == _layer_game_play_pause)
		{
			GUARD_FIELD(layer_game_play_pause);
			return new TSStagePlayLayerGamePlayPause(pstage, playerParent);
		}
		/*
		else if (name == _layer_game_background) 
		{
			GUARD_FIELD(layer_back);
			return new TSStagePlayLayerGameBackground(pstage, playerParent);
		}
		else if (name == _layer_game_doodads1)
		{
			GUARD_FIELD(layer_doodads1);
			return new TSStagePlayLayerGameDoodads1(pstage, playerParent);
		}
		else if (name == _layer_game_play_dialog)
		{
			GUARD_FIELD(layer_game_play_dialog);
			return new TSStagePlayLayerGamePlayDialog(pstage, playerParent);
		}
		else if (name == _layer_game_doodads2) 
		{
			GUARD_FIELD(layer_doodads2);
			return new TSStagePlayLayerGameDoodads2(pstage, playerParent);
		}
		*/
		else
		{
			_Assert(false);
		}
	}
	else if (pstage->name() == _stage_logo)
	{
		if (name == _layer_logo)
		{
			GUARD_FIELD(layer_logo);
			return new TSStageLogoLayerMain(pstage, playerParent);
		}
	}
	/*
	else if (pstage->name() == _stage_menu)
	{
		if (name == _layer_menu)
		{
			GUARD_FIELD(layer_menu);
			return new TSStageMenuLayerMain(pstage, playerParent);
		}
	}
	else if (pstage->name() == _stage_help)
	{
		if (name == _layer_help)
		{
			GUARD_FIELD(layer_help);
			return new TSStageHelpLayerMain(pstage, playerParent);
		}
	}
	else if (pstage->name() == _stage_options)
	{
		if (name == _layer_options)
		{
			GUARD_FIELD(layer_options);
			return new TSStageOptionsLayerMain(pstage, playerParent);
		}
	}
	else if (pstage->name() == _stage_credits)
	{
		if (name == _layer_credits)
		{
			GUARD_FIELD(layer_credits);
			return new TSStageCreditsLayerMain(pstage, playerParent);
		}
	}
	*/
	else if (pstage->name() == _stage_test)
	{
		if (name == _layer_test)
		{
			GUARD_FIELD(layer_test);
			return new TSStageTestLayerTest(pstage, playerParent);
		}
		else if (name == _layer_test_bubbles)
		{
			GUARD_FIELD(layer_test_bubbles);
			return new TSStageTestLayerTestBubbles(pstage, playerParent);
		}
	}
	_Assert(false);

	return NULL;
}

/* 
这里生成的 sprite 是全局的，可能任何Stage， 任何Layer都会使用。
这里不对他们的坐标等具象化东西进行初始化。
*/
CASprite* TSWorld::createSprite(CAStageLayer* player, const char* _name)
{
	GUARD_FUNCTION();

	CASprite* pspr = null;
	string name = _name;
	//if (player->stage()->name() == _stage_play)
	{
		//if (player->name() == _layer_game)
		{
			if (name.find("fish") == 0)
			{
				GUARD_FIELD(fish);
				pspr = new TSSpriteFish(player, name.c_str());
			}
			else if (name == "player")
			{
				GUARD_FIELD(player);
				pspr = new TSSpritePlayer(player, name.c_str());
			}
			else if (name == "bubble")
			{
				GUARD_FIELD(bubble);
				pspr = new TSSpriteBubble(player, name.c_str()); 
			}
			else if (name.find("button") == 0 || 0)
			{
				GUARD_FIELD(bubble);
				pspr = new TSSpriteButton(player, name.c_str()); 
			}
			else if (
				(name == "sea") || 
				(name == "logo") ||
				(name.find("wave") == 0) || 
				(name.find("sand") == 0) || 
				(name.find("coral") == 0) || 
				(name == "light"))
			{
				GUARD_FIELD(common_sprite);
				pspr = new TSSpriteCommon(player, name.c_str()); 
			}
			else
			{
				GUARD_FIELD(common_sprite);
				pspr = new TSSpriteCommon(player, name.c_str()); 
				//_Assert(false);
			}
		}
	}
	
	//_Trace("sprite [%s] created, p=%p", name.c_str(), pspr);

	return pspr;
}

