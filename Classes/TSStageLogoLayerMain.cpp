
#include "TSStageLogoLayerMain.h"

#include "AWorld.h"
#include "AStage.h"
#include "TSSpritePlayer.h"
#include "TSSpriteFish.h"
#include "TSSpriteBubble.h"
#include "TSSpriteCommon.h"

#define STATE_LOADING	"loading"
#define STATE_WAITING	"waiting"
#define STATE_FADING	"fading"

#define STAGE_NEXT	"play"

TSStageLogoLayerMain::TSStageLogoLayerMain(CAStage* pstage, CAStageLayer* playerParent) : CAStageLayer(pstage, playerParent)
{
	_Trace("%s allocated", __FUNCTION__);
	_pstageNext = null;
}

TSStageLogoLayerMain::~TSStageLogoLayerMain(void)
{
	_Trace("%s destroyed", __FUNCTION__);
}

void TSStageLogoLayerMain::_setState(const string& s) { _state = s; _timeState = this->getTimeNow(); }
ccTime TSStageLogoLayerMain::_getStateTime() { return this->getTimeNow() - _timeState; }

void TSStageLogoLayerMain::onEnter()
{
	GUARD_FUNCTION();

	// add a label shows "Hello World"
	// create and initialize a label
    _pInfo = CCLabelTTF::labelWithString(" ", "Arial", 16);
	// ask director the window size
	CCSize size = CAWorld::getScreenSize();

	_pInfo->setAnchorPoint(ccp(0.5f, 0.0f));

	float laby = _settings.getFloat("label_y");
	// position the label on the center of the screen
	_pInfo->setPosition( ccp(size.width / 2, laby * size.height) );

	_pInfo->setVertexZ(10);
	// add the label as a child to this layer
	this->addChild(_pInfo, 100);

	_pInfo->retain();

	CAStageLayer::onEnter();

	_setState(STATE_LOADING);
}

void TSStageLogoLayerMain::onUpdate() 
{
	CAStageLayer::onUpdate();
	
	const string& state = _state; //getState();
	if (state == STATE_LOADING)
	{
		if (null == _pstageNext)
		{
			_pstageNext = CAWorld::sharedWorld().createStage(STAGE_NEXT);
			_pstageNext->retain();
		}
		EStageLoadState s;
		s = _pstageNext->loadProgressively();
		char sz[64];
		float percent = _pstageNext->getLoadPercent();
		sprintf(sz, "%.1f%%", percent * 100.0f);
		_pInfo->setString(sz);
		_Trace("load stage returns:%d, %.2f", s, percent);

		float item_count = _settings.getFloat("items");
		int index = (int)(percent * item_count);
		//if (index > 4) index = 4;
		_Assert(index >= 0 && index <= (int)item_count);

		int i;
		for (i = 0; i < index; i++)
		{
			sprintf(sz, "p%d", i);
			if (_getNamedSpritesCount(sz))
			{
				CASprite* pspr = _getNamedSprite(sz);
				if (pspr->getState() != "ready")
				{
					pspr->setState("ready");
				}
			}
		}
		
		if (SLS_Finished == s)
		{
			_setState(STATE_WAITING);
			_pInfo->setString("");
			CASprite* pspr = _getNamedSprite("logo");
			pspr->setState("fadeout");
		}
	}
	else if (state == STATE_WAITING)
	{
		float w = _settings.getFloat("wait");
		if (_getStateTime() > w)
		{
			_setState(STATE_FADING);
			CAWorld::sharedWorld().switchStage(_pstageNext);
			_Assert(CAWorld::sharedWorld().getCurrentStage()->name() == STAGE_NEXT);
			_pstageNext->release();
			_pstageNext = null;
		}
	}
};

void TSStageLogoLayerMain::onExit()
{
	_pInfo->release();
	_pInfo = null;
	CAStageLayer::onExit();
}
