
#include "TSStagePlayLayerGamePlay.h"
#include "TSStagePlayLayerGamePlayPause.h"
#include "AWorld.h"
#include "AStage.h"
#include "AString.h"
#include "TSSpritePlayer.h"
#include "TSSpriteFish.h"
#include "TSSpriteCommon.h"
#include "TSSpriteButton.h"

TSStagePlayLayerGamePlay::TSStagePlayLayerGamePlay(CAStage* pstage, CAStageLayer* playerParent) : CAStageLayer(pstage, playerParent)
{
	_Trace("%s allocated", __FUNCTION__);
	
	_fPlayerSpeedLast = 0;

	_NullGetters();

	_animPlayerSpeed.setValid(false);
}

TSStagePlayLayerGamePlay::~TSStagePlayLayerGamePlay(void)
{
	_Trace("%s destroyed", __FUNCTION__);
}

string TSStagePlayLayerGamePlay::debuglog() 
{ 
	char sz[256];
	sprintf(sz, "ps=%s, psp=%.2f ssp=%.2f sprs=%d, state=%s", 
		_player() ? _player()->debuglog().c_str() : "N", 
		_player() ? _player()->getPos().x : 0.0f,
		this->stage()->getOffset().x,
		this->_getNamedSpritesCount(), 
		this->getCurrentState()->getLeafState()->getFullName().c_str());
	return sz;
}

bool TSStagePlayLayerGamePlay::checkCondition(CAState* from, const CATransition& trans)
{
	//root.idle:	inner condition
	//root.running:	button event
	if (CAMachine::checkCondition(from, trans))
		return true;

	string fname = from->getFullName();
	if (CAString::startWith(fname, "root.fade"))
	{
		const char* pszPose = "fadein";
		if (fname == "root.fadein")
		{
		}
		else if (fname == "root.fadeout")
		{
			pszPose = "fadeout";
		}
		else
		{
			_Assert(false);
		}
		bool result = false;
		float timeout = trans.timeout;
		if (timeout > 0 && this->getTimeNow() - from->getTimeEnter() > timeout)
		{
			result = true;
		}
		if (result)
		{
			string result = "";
			result += from->getFullName();
			result += "@";
			result += trans.condition;
			this->setConditionResult(result.c_str(), true);
		}
	}
	else if (fname == "root.diving")
	{
	}
	else if (fname == "root.wait_resume")
	{
		CAStageLayer* pl = this->_getSubLayer("game.play.pause");
		_Assert(pl);
		_Assert(trans.condition == "pause.isidle");
		if (pl->getCurrentState()->getFullName() == "root.idle")
		{
			return true;
		}
	}

	if (CAMachine::checkCondition(from, trans))
		return true;

	return false;
}

void TSStagePlayLayerGamePlay::_findNumberSprites(const char* prefix, CASprite** ppsprs, int size)
{
	int i, len = strlen(prefix);
	int count = _getNamedSpritesCount(prefix);
	_Assert(count == size);

	for (i = 0; i < count; i++)
	{
		CASprite* pspr = _getNamedSprite(prefix, i);
		string gname = pspr->getGroupName();
		char c = gname[gname.length() - 1];
		_Assert(c >= '0' && c <= '9');
		int index = c - '0';
		_Assert (index >= 0 && index < size);
		{
			ppsprs[index] = pspr;
		}
	}
}

void TSStagePlayLayerGamePlay::onStateBegin(CAState* from, void* param) 
{
	const string& fname = from->getFullName();
	_state = fname;

	if (0) ;
	else if (CAString::startWith(fname, "root.idle"))		//_onStateBeginIdle(from);
	{
		//this->setVisible(false);
		_NullGetters();
		removeAllTimelines();
	}
	else if (CAString::startWith(fname, "root.create"))		//_onStateBeginFadein(from);
	{
		//this->stage()->resetTimer();
		//this->stage()->setOffset(ccp(0, 0), 0);
		activeAllTimelines();
	}
	else if (CAString::startWith(fname, "root.fadein"))		//_onStateBeginFadein(from);
	{
		_InitGetters();

		const char* cm = "0123456789";
		
		CASprite* psprs[7];
		_findNumberSprites("dist", psprs, 6);
		_distance.init(this, psprs, 6, cm);
			
		/*
		activeAllTimeline("play_title_bar", true);
		activeTimeline("play_ui_buttons", true);
		enableTimeline("play_fishes", true);
		enableTimeline("play_rewards", true);
		*/
		//this->setVisible(true);
		_pstage->setFocus(this);
	}
	else if (CAString::startWith(fname, "root.diving"))	
	{
		stage()->setOffset(CCPointZero, 0);

		_animPlayerSpeed.init(getTimeNow(), 0, 0, 0);
		_fPlayerSpeedInPixel = _settings.getFloat("player_speed_in_percent");
		CAWorld::percent2view(_fPlayerSpeedInPixel, true);
		_fPlayerSpeedAcc = _settings.getFloat("player_speed_acc_in_distance");
		_fPlayerSpeedMax = _settings.getFloat("player_speed_max_in_percent");
		CAWorld::percent2view(_fPlayerSpeedMax, true);
		_fDistanceInPixel = _settings.getFloat("distance_in_percent");
		CAWorld::percent2view(_fDistanceInPixel, true);

		_player()->setState(PS_Dive);
		_player()->setMoveSpeed(0);
	}
	else if (CAString::startWith(fname, "root.running"))	//_onStateBeginRunning(from);
	{
		if (fname == "root.running.swiming")
		{
			//set player swim speed to normal
			_player()->setState(PS_Swiming);
		}
		else if (fname == "root.running.waitriding")
		{
			_animPlayerSpeed.init(getTimeNow(), 0.2f, _fPlayerSpeedLast, 0);
			_player()->setState(PS_WaitRiding);
		}
		else if (fname == "root.running.riding")
		{
			//get rider speed
			float fSpeedOfRedier = _fPlayerSpeedMax;
			_animPlayerSpeed.init(getTimeNow(), 0.1f, 0, fSpeedOfRedier);
			_player()->setState(PS_Riding);
		}
	}
	else if (fname == "root.pause")
	{
		CAStageLayer* pl = this->_getSubLayer("game.play.pause");
		_Assert(pl);
		pl->setConditionResult("root.idle@user.show", true);
		_pstage->setFocus(pl);
		pause();
	}
	else if (CAString::startWith(fname, "root.restart"))
	{
		//wait plPause's state in idle
		//_pstage->setFocus(this);
		resume();
		_player()->setState(PS_Prepare);
		this->_playerParent->onEvent(new CAEventCommand(this, "play.finished"));
	}
	else if (CAString::startWith(fname, "root.wait_resume"))
	{
	}
	else if (CAString::startWith(fname, "root.resume"))
	{
		//wait plPause's state in idle
		_pstage->setFocus(this);
		resume();
	}
	else if (CAString::startWith(fname, "root.over"))	
	{
		//show game over message
		//save record here
	}
	else if (CAString::startWith(fname, "root.fadeout"))
	{
		//show fadeout effects
		//transite to idle
	}
	else ;
};

//void TSStagePlayLayerGamePlay::onStateUpdate(CAState* from, size_t counter, ccTime time) 
//{
//};


void TSStagePlayLayerGamePlay::onStateEnd(CAState* from, void* param) 
{
	const string& fname = from->getFullName();
	if (0) ;
	else if (CAString::startWith(fname, "root.idle"))
	{
	}
	else if (CAString::startWith(fname, "root.fadein"))		
	{
	}
	else if (CAString::startWith(fname, "root.running"))	
	{
		if (fname == "root.running.swiming")
		{
		}
		//else if (fname == "
	}
	else if (CAString::startWith(fname, "root.pause"))	
	{
	}
	else if (CAString::startWith(fname, "root.restart"))	
	{
	}
	else if (CAString::startWith(fname, "root.resume"))
	{
	}
	else if (CAString::startWith(fname, "root.over"))	
	{
	}
	else if (CAString::startWith(fname, "root.fadeout"))	
	{
	}
	else ;
};

void TSStagePlayLayerGamePlay::pause(bool bsub)
{
	showTimeline("play_ui_buttons", false);
	CAStageLayer::pause(bsub);
}

void TSStagePlayLayerGamePlay::resume(bool bsub)
{
	CAStageLayer::resume(bsub);
	showTimeline("play_ui_buttons", true);
	_button_pause()->setState(STATE_Stand);
}

void TSStagePlayLayerGamePlay::onEnter()
{
	GUARD_FUNCTION();

	CAStageLayer::onEnter();
}

int TSStagePlayLayerGamePlay::_getDistance()
{
	int r = (int)(_player()->getPos().x / _fDistanceInPixel); 
	if (r < 0) r = 0;
	return r;
}

void TSStagePlayLayerGamePlay::_checkRewards() 
{
	CASprite* psprPlayer = _player();

	unsigned int i, count;
	count = _getNamedSpritesCount("pearl");
	for (i = 0; i < count; i++)
	{
		CASprite* pspr = _getNamedSprite("pearl", i);
		if (pspr->getState() != "dismiss")
		{
			if (psprPlayer->isCollidWith(pspr))
			{
				//_addScore(pspr);
				pspr->setState("dismiss");
			}
		}
	}
}

void TSStagePlayLayerGamePlay::_checkFishes() 
{
	this->stage()->playEffect("sfx_over");

	CASprite* psprPlayer = _player();

	unsigned int i, count;
	count = _getNamedSpritesCount("fish");
	for (i = 0; i < count; i++)
	{
		CASprite* pspr = _getNamedSprite("fish", i);
		if (psprPlayer->isCollidWith(pspr))
		{
			psprPlayer->setState("hurt");
			/*
			_nGas -= 30;
			if (_nGas < 0)
			{
				_nGas = 0;
				_player()->setState("dead");
				//setState(STATE_DYING);
				return;
			}
			*/
			//_addScore(pspr->getGroupName());
			//pspr->setState("dismiss");
		}
	}
}

void TSStagePlayLayerGamePlay::_updateStageOffset()
{
	CAStage* pstage = this->stage();

	CCSize size = CAWorld::getScreenSize();
	CCPoint pt;
	if (null == _player())
	{
		pt = CCPointZero;
	}
	else
	{
		pt = _player()->getPos();
		pt.y = 0; 
		pt.x -= size.width * _settings.getFloat("player_x");
		if ((_player()->getState() == PS_Swiming || _player()->getState() == PS_Dead) && pt.x > 0)
		{
			pstage->setOffset(pt, _settings.getFloat("view_adj"));
		}
	}
}


void TSStagePlayLayerGamePlay::onUpdate() 
{
	CAStageLayer::onUpdate();
	string fname = this->getCurrentState()->getLeafState()->getFullName();
	if (CAString::startWith(fname, "root.running"))
	{
		CASprite* psprPlayer = _player();

		if (fname == "root.running.swiming")
		{
			float speed = _fPlayerSpeedInPixel + _fPlayerSpeedAcc * _getDistance();
			if (speed > _fPlayerSpeedMax) speed = _fPlayerSpeedMax;

			_animPlayerSpeed.init(getTimeNow(), 0, _fPlayerSpeedLast, speed);
			_fPlayerSpeedLast = speed;
		}
		else if (fname == "root.running.waitriding")
		{
		}
		else if (fname == "root.running.riding")
		{
		}

		char szTemp[32];
		sprintf(szTemp, "%d", (int)_getDistance());
		_distance.setText(szTemp);
		_distance.onUpdate();

		_checkRewards();

		_updateStageOffset();
		//if (CAString::startWith(fname, "root.running"))
		if (_animPlayerSpeed.isValid() && _player())
		{
			_player()->setMoveSpeed(_animPlayerSpeed.getValue(getTimeNow()));
		}
	}
};

void TSStagePlayLayerGamePlay::onExit()
{
	CAStageLayer::onExit();
}

void TSStagePlayLayerGamePlay::onEvent(CAEvent* pevt)
{
	CAStageLayer::onEvent(pevt);

	switch (pevt->type())
	{
	case ET_Touch:
		{
			CAEventTouch* ptouch = (CAEventTouch*)pevt;
			switch (ptouch->state())
			{
			case kTouchStateGrabbed:
				{
				}
				break;
			case kTouchStateUngrabbed:
				{
				}
				break;
			}
		}
		break;
	case ET_Command:
		{
			CAEventCommand* pec = (CAEventCommand*)pevt;
			if (pec->command() == "onClick")
			{
				_Assert(pec->getSenderType() == ST_Sprite);
				CASprite* pspr = (CASprite*)pec->sender();
				string name;
				name = pspr->getModName();
				if (name == "button_pause")
				{
					this->setConditionResult("root.running@user.pause", true);
				}
			}
			else if (pec->command() == EVENT_DIVE_FINISHED)
			{
				this->setConditionResult("root.diving@pose.finished", true);
			}
			else if (pec->command() == EVENT_ONRESUME)
			{
				this->setConditionResult("root.pause@user.wait_resume", true);
			}
			else if (pec->command() == EVENT_ONRESTART)
			{
				this->setConditionResult("root.pause@user.restart", true);
			}
		}
		break;
	}
}

