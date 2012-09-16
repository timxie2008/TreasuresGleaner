
#include "TSStagePlayLayerGamePlay.h"
#include "TSStagePlayLayerGamePlayPause.h"

#include "AWorld.h"
#include "AStage.h"
#include "AString.h"
#include "AUserData.h"

#include "TSSpritePlayer.h"
#include "TSSpriteFish.h"
#include "TSSpriteCommon.h"
#include "TSSpriteButton.h"

#if defined(_DEBUG)
#define _STARS_WHALE	4
#define _STARS_DOLPHIN	2
#define _STARS_BLOCKER	1
#else
#define _STARS_WHALE	12
#define _STARS_DOLPHIN	6
#define _STARS_BLOCKER	5
#endif

TSStagePlayLayerGamePlay::TSStagePlayLayerGamePlay(CAStage* pstage, CAStageLayer* playerParent) : CAStageLayer(pstage, playerParent)
{
	_Trace("%s allocated", __FUNCTION__);
	
	_nCollected = 0;
	_ptLastBlocker = CCPointZero;
	_fOffsetDolphin = 0;
	_fOffsetWhale = 0;
	_bCreateDolphin = false;
	_bCreateWhale = false;

	memset(_psprIndicators, 0, sizeof(_psprIndicators));

	_NullGetters();
}

TSStagePlayLayerGamePlay::~TSStagePlayLayerGamePlay(void)
{
	_Trace("%s destroyed", __FUNCTION__);
}

string TSStagePlayLayerGamePlay::debuglog() 
{ 
	char sz[1024];
	string sprinfo = this->_sprite_container_debug_log().c_str();
	if (sprinfo.size())
	{
		sprinfo += " ";
	}
	sprintf(sz, "(%.2f,%.2f) hs=%.2f,vs=%.2f c=%d ps=%s psp=%.2f ssp=%.2f sprs=%d, state=%s,", 
		_player() ? _player()->getPos().x : 0,
		_player() ? _player()->getPos().y : 0,
		_player() ? _player()->getHMoveSpeed() : 0.0f,
		_player() ? _player()->getVMoveSpeed() : 0.0f,
		_nCollected, _player() ? _player()->debuglog().c_str() : "N", 
		_player() ? _player()->getPos().x : 0.0f,
		this->stage()->getOffset().x,
		this->_getNamedSpritesCount(), 
		this->getCurrentState()->getLeafState()->getFullName().c_str());

#if defined(_DEBUG)
	strcat(sz, sprinfo.c_str());
#endif

	return sz;
}

TSSpriteCommon* TSStagePlayLayerGamePlay::_createCommonSprite(const char* name, const char* state, const CCPoint& pos, bool autoremove, float scale)
{
	TSSpriteCommon* pspr = new TSSpriteCommon(this, name);
	pspr->setPos(pos);
	pspr->setScl(scale);
	pspr->setState(state);
	if (autoremove)
	{
		pspr->setDeadPose(state);
	}
	return pspr;
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
		_Info("check result timeout(%.2f-%.2f > %.2f): %s => %s", this->getTimeNow(), from->getTimeEnter(), timeout, fname.c_str(), result ? "true" : "false");
		return result;
	}
	else if (fname == "root.diving")
	{
	}
	else if (fname == "root.over")
	{
		CASprite* pspr = _getNamedSprite("lab_gameover");
		if (pspr->isAnimationDone() && pspr->getState() == "gameover_fadeout")
			return true;
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
		strings parts;
		CAString::split(gname, "-", parts);
		string order = parts[parts.size() - 1];
		int index = atoi(order.c_str());
		_Assert(index >= 0 && index < size);
		_Assert(null == ppsprs[index]);
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
	else if (CAString::startWith(fname, "root.idle"))
	{
		//this->setVisible(false);
		_NullGetters();
		removeAllTimelines();
		killAllSprites();
	}
	else if (CAString::startWith(fname, "root.create"))
	{
		//_Assert(0 == this->_getNamedSpritesCount());

		strings excludes;
		excludes.push_back("gameover_bar");
		activeAllTimelines(&excludes);

		_nCollected = 0;

		int seed = _settings.getInteger("traceline_seed");
		float left = _settings.getFloat("traceline_left");
		float top = _settings.getFloat("traceline_top");
		float bottom = _settings.getFloat("traceline_bottom");
		float node_density = _settings.getFloat("traceline_node_density");
		float node_rand_range = _settings.getFloat("traceline_node_rand_range");
		float point_density = _settings.getFloat("traceline_point_density");
		int seg_max = _settings.getInteger("traceline_yseg_max");
		int seg_range = _settings.getInteger("traceline_yseg_range");

		_traceline_coin2pearl = _settings.getInteger("traceline_coin2pearl");
		_Assert(_traceline_coin2pearl > 1 && _traceline_coin2pearl < 400);

		float point_line;
		float point_line_delta;
		float point_gap;
		float point_gap_delta;

		point_line = _settings.getFloat("traceline_point_line");
		point_gap = _settings.getFloat("traceline_point_gap");
		point_line_delta = _settings.getFloat("traceline_point_line_delta");
		point_gap_delta = _settings.getFloat("traceline_point_gap_delta");
		_Assert(point_line > point_line_delta);
		_Assert(point_line > point_gap);
		_Assert(point_gap > point_gap_delta);

		_traceline_blocker_k = _settings.getFloat("traceline_blocker_k");
		_traceline_blocker_dy_percent_from_center = _settings.getFloat("traceline_blocker_dy_percent_from_center");
		_traceline_block_density = _settings.getFloat("traceline_block_density");
		CAWorld::percent2view(_traceline_block_density, true);

		_traceline_dolphin_density = _settings.getFloat("traceline_dolphin_density");
		_traceline_whale_density = _settings.getFloat("traceline_whale_density");
		CAWorld::percent2view(_traceline_dolphin_density, true);
		CAWorld::percent2view(_traceline_whale_density, true);
		_fOffsetDolphin = 0;
		_fOffsetWhale = 0;
		_bCreateDolphin = false;
		_bCreateWhale = false;

		_ptLastBlocker = CCPointZero;
		_traceline.init(seed, left, top, bottom, node_density, node_rand_range, point_density, seg_max, seg_range);
		_traceline.setSegPoints(point_line, point_line_delta, point_gap, point_gap_delta);
	}
	else if (CAString::startWith(fname, "root.fadein"))
	{
		_Info("spr count = %d", this->_getNamedSpritesCount());

		_InitGetters();

		const char* cm = "0123456789";
		
		CASprite* psprs[7];
		memset(psprs, 0, sizeof(psprs));
		_findNumberSprites("dist", psprs, 6);
		_distance.init(this, psprs, 6, cm);
		
		memset(_psprIndicators, 0, sizeof(_psprIndicators));
		_findNumberSprites("score", _psprIndicators, SIZE_OF_ARRAY(_psprIndicators));
		_nCollected = 0;
		_updateScoreBar();

		_pstage->setFocus(this);
	}
	else if (CAString::startWith(fname, "root.diving"))	
	{
		stage()->setOffset(CCPointZero, 0);

		float fPlayerSpeedInPixel = _settings.getFloat("player_speed_in_percent");
		CAWorld::percent2view(fPlayerSpeedInPixel, true);
		float fPlayerSpeedAcc = _settings.getFloat("player_speed_acc_in_distance");
		float fPlayerSpeedMax = _settings.getFloat("player_speed_max_in_percent");
		CAWorld::percent2view(fPlayerSpeedMax, true);

		_player()->setSpeedInfo(fPlayerSpeedInPixel, fPlayerSpeedAcc, fPlayerSpeedMax);
		_player()->setState(PS_Dive);
		_player()->setHMoveSpeed(0);
		_player()->setVMoveSpeed(0);

		_fDistanceInPixel = _settings.getFloat("distance_in_percent");
		CAWorld::percent2view(_fDistanceInPixel, true);
	}
	else if (CAString::startWith(fname, "root.running"))
	{
		//set player swim speed to normal
		_player()->setState(PS_Swiming);
		_button_pause()->setVisible(true);
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
		CAUserData::sharedUserData().setInteger("last_score", this->_getDistance());
		_player()->setState(PS_Prepare);
		
		setTimelineState("gameover_bar", "gameover_fadeout");
	}
	else if (CAString::startWith(fname, "root.fadeout"))
	{
		//show fadeout effects
		//transite to idle
		//this->removeAllSpritesByGroupName("pearl");
		//this->removeAllSpritesByGroupName("blocker");

		this->_playerParent->onEvent(new CAEventCommand(this, "play.finished"));
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

void TSStagePlayLayerGamePlay::_updateScoreBar()
{
	int i;
	int n = _nCollected / _traceline_coin2pearl;
	int p = 0;
	for (i = 0; i < SIZE_OF_ARRAY(_psprIndicators); i++)
	{
		CASprite* pspr = _psprIndicators[i];
		if (pspr && pspr->getModName() == "pearl")
		{
			if (p < n)
			{
				if (pspr->getState() != "indicator_golden")
				{
					pspr->setState("indicator_golden");
				}
			}
			else
			{
				if (pspr->getState() != "indicator_white")
				{
					pspr->setState("indicator_white");
				}
			}
			p++;
		}
	}
}

void TSStagePlayLayerGamePlay::_addCollected(int c)
{
	_nCollected += c;

	int cmax = _traceline_coin2pearl * (SIZE_OF_ARRAY(_psprIndicators) - 4);
	if (_nCollected < 0) 
	{
		_nCollected = 0;
	}
	else if (_nCollected > cmax)
	{
		_nCollected = cmax;
	}

	if (_nCollected <= 0)
	{
		_player()->setState("dead");
		
		activeTimeline("gameover_bar");
		_button_pause()->setVisible(false);
	}
	else if (_nCollected / _traceline_coin2pearl >= _STARS_DOLPHIN)
	{
		//if we can put a whale | dolphin
		_bCreateDolphin = false;
		_bCreateWhale = false;
		float x = stage()->getOffset().x;
		if (_nCollected / _traceline_coin2pearl < _STARS_WHALE)
		{
			if (x > _fOffsetDolphin + _traceline_dolphin_density)
			{
				_fOffsetDolphin = x;
				_bCreateDolphin = true;
			}
		}
		else
		{
			if (x > _fOffsetWhale + _traceline_whale_density)
			{
				_fOffsetWhale = x;
				_bCreateWhale = true;
			}
		}
	}

	_updateScoreBar();
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
				_addCollected(1);
				pspr->setState("dismiss");
			}
		}
	}

	CCPoint posCollid;
	bool bCollided = false;
	char* szRewards[] = { "rdolphin", "rwhale" };
	string strReward = "";
	int j;
	for (j = 0; !bCollided && j < 2; j++)
	{
		count = _getNamedSpritesCount(szRewards[j]);
		for (i = 0; !bCollided && i < count; i++)
		{
			CASprite* pspr = _getNamedSprite(szRewards[j], i);
			if (psprPlayer->isCollidWith(pspr))
			{
				TSSpriteCommon* pspr;
				pspr = _createCommonSprite("cloud_01", "cloud_01_01", psprPlayer->getPos(), true, 1.2f);
				this->addSprite(pspr);

				pspr = _createCommonSprite("cloud_01", "cloud_01_02", pspr->getPos(), true, 1.3f);
				this->addSprite(pspr);

				strReward = szRewards[j] + 1;
				bCollided = true;
			}
		}
	}
	if (bCollided)
	{
		this->removeAllSpritesByGroupName("reward");
		((TSSpritePlayer*)psprPlayer)->ride(strReward);
	}
}

void TSStagePlayLayerGamePlay::_checkBlockers() 
{
	//this->stage()->playEffect("sfx_over");

	CASprite* psprPlayer = _player();

	unsigned int i, count;
	count = _getNamedSpritesCount("blocker");
	for (i = 0; i < count; i++)
	{
		CASprite* pspr = _getNamedSprite("blocker", i);
		if (pspr->getState() != "dismiss")
		{
			if (psprPlayer->isCollidWith(pspr))
			{
				if (psprPlayer->getState() == PS_Swiming ||
					psprPlayer->getState() == PS_RidingDolphin ||
					psprPlayer->getState() == PS_RidingWhale || 
					0
					)
				{
					psprPlayer->setState("hurt");
				}
				_addCollected(-(_STARS_BLOCKER *  _traceline_coin2pearl));
				pspr->setState("dismiss");
			}
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
		if ((_player()->getState() != PS_Prepare &&
			_player()->getState() != PS_Fadeout &&
			_player()->getState() != PS_Dive) 
			&& pt.x > 0)
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
		int distance = _getDistance();
		_player()->setDistance4CalculatingSpeed((float)distance);

		char szTemp[32];
		sprintf(szTemp, "%d", distance);
		_distance.setText(szTemp);
		_distance.onUpdate();

		_checkRewards();
		_checkBlockers();

		_updateStageOffset();

		CCPoint ptOffset = stage()->getOffset();
		CCPoint ptLast = _traceline.getLastTracePoint();
		CAWorld::percent2view(ptLast);
		CCSize size = CAWorld::getScreenSize();
		while (ptLast.x < ptOffset.x + size.width * 1.8f)
		{
			TSSpriteCommon* pspr;

			Vector3 v0, v1, vc, vd, vr;
			
			CCPoint pt;
			int flag = _traceline.getNextTracePoint(pt);
			CAWorld::percent2view(pt);
			
			v0.x = pt.x;
			v0.y = pt.y;
			v0.z = 0;
			v1.x = ptLast.x;
			v1.y = ptLast.y;
			v1.z = 0;
	
			ptLast = pt;

			CCRect rect;
			rect.origin = ccp(-0.2f, 0);
			rect.size.width = 4.0f;
			rect.size.height = 1.0f;

			if (0 == flag)
			{
				if (_bCreateDolphin || _bCreateWhale)
				{
					float scale = 0.7f;

					if (_bCreateDolphin)
					{
						pspr = _createCommonSprite("dolphin", "swim", pt, false, scale);
						pspr->setLiveArea(rect);
						pspr->setFollowCamera(true);
						pspr->setGroupName("reward-rdolphin");
						this->addSprite(pspr);

						pspr = _createCommonSprite("bubble", "ellipse_dolphin", pt, false, scale);
						pspr->setGroupName("reward-rbubble");
						pspr->setLiveArea(rect);
						pspr->setFollowCamera(true);
						this->addSprite(pspr);
					}
					else
					{
						pspr = _createCommonSprite("whale", "swim", pt, false, scale);
						pspr->setLiveArea(rect);
						pspr->setFollowCamera(true);
						pspr->setGroupName("reward-rwhale");
						this->addSprite(pspr);

						pspr = _createCommonSprite("bubble", "ellipse_whale", pt, false, scale);
						pspr->setGroupName("reward-rbubble");
						pspr->setLiveArea(rect);
						pspr->setFollowCamera(true);
						this->addSprite(pspr);
					}

					_bCreateDolphin = false;
					_bCreateWhale = false;
				}
				else
				{
					pspr = _createCommonSprite("pearl", "golden", pt);
					pspr->setLiveArea(rect);
					pspr->setFollowCamera(true);
					this->addSprite(pspr);
				}
			}
			//random some blockers
			if (_traceline.getSegmentsCount() <= 0)
			{
				continue;
			}

			vd = v1 - v0;
			if (_Abs(vd.y / vd.x) > _traceline_blocker_k)
			{
				continue;
			}

			if (v0.x - _ptLastBlocker.x < _traceline_block_density)
			{
				continue;
			}

			//get mirror point of pt
			vc.x = pt.x;
			vc.y = size.height / 2.0f;
			vc.z = 0;
			vr = vc - v0;
			float len = vr.length();

			if (len < size.height * _traceline_blocker_dy_percent_from_center)
			{
				continue;
			}

			len = (CAUtils::Rand() * 0.8f + 1.8f) * len;
			vr.normalize();
			vr *= len;
			vr = v0 + vr;
			if (vr.y < _traceline.bottom() * size.height || vr.y > _traceline.top() * size.height)
			{
				continue;
			}

			//try to create a blocker
			CCPoint ptBlocker;
			ptBlocker.x = vr.x;
			ptBlocker.y = vr.y;
			_ptLastBlocker = ptBlocker;

			int n = (int)(CAUtils::Rand() * 5.0f);
			char szBlocker[32];
			sprintf(szBlocker, "blocker-%02d", n + 1);

			pspr = _createCommonSprite(szBlocker, "stand", ptBlocker);
			pspr->setLiveArea(rect);
			pspr->setFollowCamera(true);
			this->addSprite(pspr);
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
			else if (pec->command() == "over")
			{
				this->setConditionResult("root.running@player.dead", true);
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

