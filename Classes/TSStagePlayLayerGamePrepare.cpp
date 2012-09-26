#include "TSStagePlayLayerGamePrepare.h"
#include "TSStagePlayLayerGamePrepareQuit.h"

#include "AWorld.h"
#include "AStage.h"
#include "AString.h"
#include "AUserData.h"

#include "TSSpritePlayer.h"
#include "TSSpriteCommon.h"
#include "TSSpriteButton.h"
#include "TSSpriteBubble.h"

TSStagePlayLayerGamePrepare::TSStagePlayLayerGamePrepare(CAStage* pstage, CAStageLayer* playerParent) : CAStageLayer(pstage, playerParent)
{
	_Trace("%s allocated", __FUNCTION__);
	_NullGetters();

	_bScoreDirty = true;
	_score_last = 0;
	_score_max = 0;
}

TSStagePlayLayerGamePrepare::~TSStagePlayLayerGamePrepare(void)
{
	_Trace("%s destroyed", __FUNCTION__);
}

string TSStagePlayLayerGamePrepare::debuglog() 
{ 
	char sz[256];
	sprintf(sz, "sprs=%d, state=%s", this->_getNamedSpritesCount(), 
		this->getCurrentState()->getLeafState()->getFullName().c_str());
	return sz;
}

bool TSStagePlayLayerGamePrepare::checkCondition(CAState* from, const CATransition& trans)
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
		else
		{
			//check 3 controls pose
			CASprite* psprs[] = 
			{
				_label_title(),
				_button_play(),
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


void TSStagePlayLayerGamePrepare::_findNumberSprites(const char* prefix, CASprite** ppsprs, int size)
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

void TSStagePlayLayerGamePrepare::onStateBegin(CAState* from, void* param) 
{
	const string& fname = from->getFullName();
	if (0) ;
	else if (CAString::startWith(fname, "root.idle"))
	{
		_NullGetters();
		this->removeAllTimelines();
	}
	else if (CAString::startWith(fname, "root.create"))
	{
		//_pstage->resetTimer();
		_pstage->setOffset(ccp(0, 0), 0.0f);
		this->activeAllTimelines();
	}
	else if (CAString::startWith(fname, "root.fadein"))
	{
		_InitGetters();

		this->stage()->playMusic("music_back_1");

		_bScoreDirty = true;
		_score_last = CAUserData::sharedUserData().getInteger("last_score");;
		_score_max = CAUserData::sharedUserData().getInteger("max_score");;
		if (_score_last > _score_max)
		{
			_score_max = _score_last;
			CAUserData::sharedUserData().setInteger("max_score", _score_max);
		}

		const char* cm = "0123456789m+";
		
#define NUMBER_COUNT 7
		CASprite* psprs[NUMBER_COUNT];
		_findNumberSprites("dist_last", psprs, NUMBER_COUNT);
		_dist_last.init(this, psprs, NUMBER_COUNT, cm);
		_findNumberSprites("dist_max", psprs, NUMBER_COUNT);
		_dist_max.init(this, psprs, NUMBER_COUNT, cm);

		CASprite* psprsStatic[] = 
		{
			_label_title(),
			_info_dist_back(),
			_label_dist_last(),
			_label_dist_max(),
			null,
		};
		CASprite* psprsControls[] =
		{
			_button_play(),
			null,
		};
		_setSpritesState(STATE_Fadein, psprsStatic);
		_setSpritesState(STATE_Fadein, psprsControls);

		_button_sound()->setState(stage()->isSoundMute() ? "off" : "on");
	}
	else if (CAString::startWith(fname, "root.running"))
	{
		_pstage->setFocus(this);
		_label_title()->setState("stand");
	}
	else if (CAString::startWith(fname, "root.onquit"))
	{
		CAStageLayer* pl = this->_getSubLayer("game.prepare.quit");
		_Assert(pl);
		pl->setConditionResult("root.idle@user.show", true);
		_pstage->setFocus(pl);
		//pause();
	}
	else if (CAString::startWith(fname, "root.onexit"))
	{
		CCDirector::sharedDirector()->end();
	}
	else if (CAString::startWith(fname, "root.onplay"))
	{
		_Assert(this->_playerParent);
		this->_playerParent->onEvent(new CAEventCommand(this, "onPlay"));
	}
	else if (CAString::startWith(fname, "root.fadeout"))
	{
		CASprite* psprsStatic[] = 
		{
			_label_title(),
			_info_dist_back(),
			_label_dist_last(),
			_label_dist_max(),
			_prepare_player(),
			_prepare_whale(),
			_prepare_dolphin(),
			null,
		};
		CASprite* psprsControls[] =
		{
			_button_play(),
			null,
		};
		_setSpritesState(STATE_Fadeout, psprsStatic);
		_setSpritesState(STATE_Fadeout, psprsControls);
		
		//_button_sound()->setState(stage()->isSoundMute() ? "off" : "on");

		_prepare_bubble_whale()->setState("ellipse_whale_fadeout");
		_prepare_bubble_dolphin()->setState("ellipse_dolphin_fadeout");

		_dist_last.setState(STATE_Fadeout);
		_dist_max.setState(STATE_Fadeout);
	}
	else ;
};

void TSStagePlayLayerGamePrepare::onStateEnd(CAState* from, void* param) 
{
	const string& fname = from->getFullName();
	if (0) ;
	else if (CAString::startWith(fname, "root.idle"))		//_onStateEndIdle(from);
	{
	}
	else if (CAString::startWith(fname, "root.create"))		//_onStateEndFadein(from);
	{
	}
	else if (CAString::startWith(fname, "root.fadein"))		//_onStateEndFadein(from);
	{
		//_button_music()->setState(stage()->isMusicMute() ? "off" : "on");
		_button_sound()->setState(stage()->isSoundMute() ? "off" : "on");
	}
	else if (CAString::startWith(fname, "root.running"))	//_onStateEndRunning(from);
	{
	}
	else if (CAString::startWith(fname, "root.onquit"))		//_onStateEndOnShop(from);
	{
	}
	else if (CAString::startWith(fname, "root.onplay"))		//_onStateEndOnPlay(from);
	{
	}
	else if (CAString::startWith(fname, "root.fadeout"))	//_onStateEndFadeout(from);
	{
	}
	else ;
};

void TSStagePlayLayerGamePrepare::show(bool s)
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

void TSStagePlayLayerGamePrepare::onEnter()
{
	GUARD_FUNCTION();

    //CCLabelTTF::labelWithString("Crazy Amber", "Arial", 16);

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

void TSStagePlayLayerGamePrepare::onUpdate() 
{
	GUARD_FUNCTION();

	CAStageLayer::onUpdate();
	if (this->getCurrentState()->getFullName() == "root.running")
	{
		if (_bScoreDirty)
		{
			char szNumber[32];

			sprintf(szNumber, "%dm", _score_last);
			_dist_last.setText(szNumber);
			_dist_last.onUpdate();

			sprintf(szNumber, "%dm", _score_max);
			_dist_max.setText(szNumber);
			_dist_max.onUpdate();
		}

		int c = this->_getNamedSpritesCount("bubble");
		if (c < 20)
		{
			//TSSpriteBubble* pspr;
			//pspr = (TSSpriteBubble*)CAWorld::sharedWorld().createSprite(this, "bubble");
			TSSpriteCommon* pspr = new TSSpriteCommon(this, "bubble");

			CCPoint pos;
			pos.x = CAUtils::Rand() * 0.8f + 0.1f;
			pos.y = CAUtils::Rand() * 0.1f;
			pos = stage()->getPointFromPercent(pos, false);

			pspr->setPos(pos);

			float sc = CAUtils::Rand() * 0.2f + 0.1f;
			pspr->setScl(sc * 2.0f);
			//pspr->setMoveDirection(270.0f);
			//sc = CAUtils::Rand(0.5f, 1.1f);
			//pspr->setMoveSpeed(-sc * 70.0f);

			pspr->setZOrder(_label_title()->getZOrder() - CAUtils::Rand(0.0f, 0.3f));

			this->addSprite(pspr);
			char* szPose = "pop_slow";
			pspr->setState(szPose);
			pspr->setDeadPose(szPose);
		}
	}
};

void TSStagePlayLayerGamePrepare::onExit()
{
	CAStageLayer::onExit();
}

void TSStagePlayLayerGamePrepare::onEvent(CAEvent* pevt)
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
	case ET_Key:
		{
			CAEventKey* pek = (CAEventKey*)pevt;
			if (KE_Back == pek->key()) // || KE_Menu == pek->key())
			{
				this->setConditionResult("root.running@user.quit", true);
			}
		}
		break;
	case ET_Command:
		{
			CAEventCommand* pec = (CAEventCommand*)pevt;
			if (pec->command() == EVENT_ONYES)
			{
				this->setConditionResult("root.onquit@user.yes", true);
			}
			else if (pec->command() == EVENT_ONNO)
			{
				this->setConditionResult("root.onquit@user.no", true);
			}
			else if (pec->command() == "onClick")
			{
				_Assert(pec->getSenderType() == ST_Sprite);
				CASprite* pspr = (CASprite*)pec->sender();
				string name;
				name = pspr->getModName();
				if (name == "button_play")
				{
					this->setConditionResult("root.running@user.play", true);
				}
				else if (name == "button_sound")
				{
					if (stage()->isSoundMute() || stage()->isMusicMute())
					{
						stage()->enableSound(true);
						stage()->enableMusic(true);
						_button_sound()->setState("on");
					}
					else
					{
						stage()->enableSound(false);
						stage()->enableMusic(false);
						_button_sound()->setState("off");
					}
				}
			}
		}
		break;
	}
}

