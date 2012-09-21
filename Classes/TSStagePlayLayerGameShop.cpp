
#include "TSStagePlayLayerGameShop.h"
#include "AWorld.h"
#include "AStage.h"
#include "AString.h"
#include "TSSpritePlayer.h"
#include "TSSpriteCommon.h"
#include "TSSpriteButton.h"

TSStagePlayLayerGameShop::TSStagePlayLayerGameShop(CAStage* pstage, CAStageLayer* playerParent) : CAStageLayer(pstage, playerParent)
{
	_Trace("%s allocated", __FUNCTION__);
	//_initGetters();
}

TSStagePlayLayerGameShop::~TSStagePlayLayerGameShop(void)
{
	_Trace("%s destroyed", __FUNCTION__);
}

string TSStagePlayLayerGameShop::debuglog() 
{ 
	return this->getCurrentState()->getLeafState()->getFullName(); 
}

bool TSStagePlayLayerGameShop::checkCondition(CAState* from, const CATransition& trans)
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
		if (timeout > 0.00001f && this->getTimeNow() - from->getTimeEnter() > timeout)
		{
			result = true;
		}
		else
		{
			//check 3 controls pose
			CASprite* psprs[] = 
			{
				_button_restart(),
				_button_resume(),
				null,
			};
			result = _checkSpritePoseFinished(pszPose, psprs);
		}
		return result;
	}

	if (CAMachine::checkCondition(from, trans))
		return true;

	return false;
}

void TSStagePlayLayerGameShop::_onStateBeginIdle(CAState* pstate) 
{ 
	//this->setVisible(false);
}

void TSStagePlayLayerGameShop::_onStateEndIdle(CAState* pstate) 
{ 
}

void TSStagePlayLayerGameShop::_onStateBeginFadein(CAState* pstate) 
{ 
	CASprite* psprsControls[] =
	{
		_label_pause_title(),
		_panel_back(),
		_button_restart(),
		_button_resume(),
		//_button_music(),
		//_button_sound(),
		null,
	};
	_setSpritesState(STATE_Fadein, psprsControls);
	_button_music()->setState(stage()->isMusicMute() ? "off" : "on");
	_button_sound()->setState(stage()->isSoundMute() ? "off" : "on");

	//this->setVisible(true);
}

void TSStagePlayLayerGameShop::_onStateEndFadein(CAState* pstate) 
{ 
}

void TSStagePlayLayerGameShop::_onStateBeginRunning(CAState* pstate) 
{ 
	//update numbers begin
}

void TSStagePlayLayerGameShop::_onStateEndRunning(CAState* pstate) 
{ 
}

void TSStagePlayLayerGameShop::_onStateBeginOnResume(CAState* pstate) 
{ 
	_Assert(this->_playerParent);
	this->_playerParent->onEvent(new CAEventCommand(this, "onOnResume"));
}

void TSStagePlayLayerGameShop::_onStateEndOnResume(CAState* pstate) 
{ 
}

void TSStagePlayLayerGameShop::_onStateBeginOnRestart(CAState* pstate) 
{ 
	_Assert(this->_playerParent);
	this->_playerParent->onEvent(new CAEventCommand(this, "onResart"));
}

void TSStagePlayLayerGameShop::_onStateEndOnRestart(CAState* pstate) 
{ 
}

void TSStagePlayLayerGameShop::_onStateBeginFadeout(CAState* pstate) 
{ 
	CASprite* psprsControls[] =
	{
		_label_pause_title(),
		_panel_back(),
		_button_restart(),
		_button_resume(),
		_button_music(),
		_button_sound(),
		null,
	};
	_setSpritesState(STATE_Fadeout, psprsControls);

	//remove all fishes here
	unsigned int i, count;
	count = _getNamedSpritesCount("fish");
	for (i = 0; i < count; i++)
	{
		CASprite* pspr = _getNamedSprite("fish", i);
		pspr->killMyself();
	}
}

void TSStagePlayLayerGameShop::_onStateEndFadeout(CAState* pstate) 
{
	
}

void TSStagePlayLayerGameShop::onStateBegin(CAState* from, void* param) 
{
	const string& fname = from->getFullName();
	if (0) ;
	else if (CAString::startWith(fname, "root.idle"))		_onStateBeginIdle(from);
	else if (CAString::startWith(fname, "root.fadein"))		_onStateBeginFadein(from);
	else if (CAString::startWith(fname, "root.running"))	_onStateBeginRunning(from);
	else if (CAString::startWith(fname, "root.onresume"))	_onStateBeginOnResume(from);
	else if (CAString::startWith(fname, "root.onrestart"))	_onStateBeginOnRestart(from);
	else if (CAString::startWith(fname, "root.fadeout"))	_onStateBeginFadeout(from);
	else ;
};

void TSStagePlayLayerGameShop::onStateEnd(CAState* from, void* param) 
{
	const string& fname = from->getFullName();
	if (0) ;
	else if (CAString::startWith(fname, "root.idle"))		_onStateEndIdle(from);
	else if (CAString::startWith(fname, "root.fadein"))		_onStateEndFadein(from);
	else if (CAString::startWith(fname, "root.running"))	_onStateEndRunning(from);
	else if (CAString::startWith(fname, "root.onresume"))	_onStateEndOnResume(from);
	else if (CAString::startWith(fname, "root.onrestart"))	_onStateEndOnRestart(from);
	else if (CAString::startWith(fname, "root.fadeout"))	_onStateEndFadeout(from);
	else ;
};

void TSStagePlayLayerGameShop::show(bool s)
{
	if (s)
	{
		this->navigateToState(getTimeNow(), this->getCurrentState(), "root.idle");
		this->setConditionResult("root.idle@user.show", true);
	}
	else
	{
		this->navigateToState(getTimeNow(), this->getCurrentState(), "root.fadeout");
	}
}

void TSStagePlayLayerGameShop::onEnter()
{
	GUARD_FUNCTION();

	CAStageLayer::onEnter();
}

static int _goNear(int cur, int to, int range, int step)
{
	if (cur == to) return cur;
	if ((cur - to < 0 && to - cur > (range >> 1)) ||
		(cur - to > 0 && cur - to < (range >> 1)))
	{
		cur += range - step;
	}
	else
	{
		cur += step;
	}
	cur %= range;

	return cur;
}

void TSStagePlayLayerGameShop::_updateNumber(const char* prefix, int nValue)
{
	if (nValue < 0)
		return;

	unsigned int i, count;

	int len = strlen(prefix);
	count = _getNamedSpritesCount(prefix);
	if (count > 0)
	{
		_Assert(count == 6);
		char szFmt[32];
		sprintf(szFmt, "%%%02dd", count);
		char szScore[32];
		sprintf(szScore, szFmt, nValue);
		for (i = 0; i < count; i++)
		{
			CASprite* pspr = _getNamedSprite(prefix, i);
			string gname = pspr->getGroupName();
			strings items;
			CAString::split(gname, "-", items);
			const char* szIndex = items[items.size() - 1].c_str();
			if (*szIndex >= '0' && *szIndex <= '9')
			{
			}
			else
			{
				continue;
			}
			int index = atoi(szIndex);
			char szPose[16];
			strcpy(szPose, pspr->getCurrentPose()->name().c_str());

			int need = szScore[index] - '0';
			int now = szPose[0] - '0';
			
			int should = _goNear(now, need, 10, 1);
			szPose[0] = '0' + should;
			_Assert(szPose[0] >= '0' && szPose[0] <= '9');
			pspr->switchPose(szPose);
		}
	}
}

void TSStagePlayLayerGameShop::onUpdate() 
{
	CAStageLayer::onUpdate();
};

void TSStagePlayLayerGameShop::onExit()
{
	CAStageLayer::onExit();
}

void TSStagePlayLayerGameShop::onEvent(CAEvent* pevt)
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
				if (name == "button_restart")
				{
					this->setConditionResult("root.running@user.restart", true);
				}
				else if (name == "button_resume")
				{
					this->setConditionResult("root.running@user.resume", true);
				}
				else if (name == "button_music")
				{
					if (stage()->isMusicMute())
					{
						stage()->enableMusic(true);
						_button_music()->setState("on");
					}
					else
					{
						stage()->enableMusic(false);
						_button_music()->setState("off");
					}
				}
				else if (name == "button_sound")
				{
					if (stage()->isSoundMute())
					{
						stage()->enableSound(true);
						_button_sound()->setState("on");
					}
					else
					{
						stage()->enableSound(false);
						_button_sound()->setState("off");
					}
				}
			}
		}
		break;
	}
}

