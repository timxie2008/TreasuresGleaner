
#include "TSSpritePlayer.h"
#include "TSSpriteBubble.h"
#include "AWorld.h"
#include "AStage.h"
#include "AStageLayer.h"
#include "TSSpriteCommon.h"

static const char* POSE_BORN = "born";
static const char* POSE_SWIM = "swim";
static const char* POSE_HURT = "hurt";
static const char* POSE_RIDING_DOLPHIN = "ride_dolphin";
static const char* POSE_RIDING_WHALE = "ride_whale";
static const char* POSE_DEAD = "dead";

TSSpritePlayer::TSSpritePlayer(CAStageLayer* player, const char* name) : CASprite(player, name)
{
	_Trace("player created");

	_psprRider = null;

	_fPlayerSpeedLast = 0;

	_fPlayerSpeedInPixel = 0;
	_fPlayerSpeedAcc = 0;
	_fPlayerSpeedMax = 0;
	_animPlayerSpeed.setValid(false);

	_direction = 0;
	_toRotation = 0;
	_timeTouchEvent = 0;
	_rectYard = _settings().getRect("yard");
	CAWorld::percent2view(_rectYard);

	int i;
	char* szNames[] = { "player", "dolphin", "whale" };
	for (i = 0; i < 3; i++)
	{
		string name;
		name = szNames[i];	name += "_a";			_swim_a[i] = _settings().getFloat(name.c_str());
		name = szNames[i];	name += "_v0";			_swim_v0[i] = _settings().getFloat(name.c_str());
		name = szNames[i];	name += "_hs_up";		_swim_hs_up[i] = _settings().getFloat(name.c_str());
		name = szNames[i];	name += "_hs_down";		_swim_hs_down[i] = _settings().getFloat(name.c_str());
		name = szNames[i];	name += "_app_frames";	_swim_app_frames[i] = _settings().getFloat(name.c_str());
	}

	_speed_weight_dolphin = _settings().getFloat("speed_weight_dolphin");
	_speed_weight_whale = _settings().getFloat("speed_weight_whale");

	_posOffsetDolphin = _settings().getPoint("offset_dolphin");
	CAWorld::percent2view(_posOffsetDolphin);
	_posOffsetWhale = _settings().getPoint("offset_whale");
	CAWorld::percent2view(_posOffsetWhale);

	this->setTouchable(true);
	this->setControlOrder(100);
	player->addControl(this);
}

TSSpritePlayer::~TSSpritePlayer(void)
{
	_Trace("player deleted");
}

void TSSpritePlayer::release(void)
{
	CASprite::release();
}

void TSSpritePlayer::_onAnimationStart()
{
	_toRotation = 0;
}

void TSSpritePlayer::_onAnimationStop()
{
	GUARD_FUNCTION();

	CASpriteModelPose* ppose = this->_currentPose;
	if (_state == PS_Dive && ppose->name() == POSE_BORN)
	{
		_pLayer->onEvent(new CAEventCommand(this, EVENT_DIVE_FINISHED));
		setState(PS_Swiming);
		_direction = -1;
		_timeTouchEvent = _pLayer->getTimeNow();
	}
	else if (_state == PS_Dead && ppose->name() == POSE_DEAD)
	{
		GUARD_FIELD(_event_command);
		setState("");
		this->setMoveSpeed(0);
		_pLayer->onEvent(new CAEventCommand(this, "over"));
	}
}

void TSSpritePlayer::_createBubbles(int c, bool bLow)
{
	int i;

	for (i = 0; i < c; i++)
	{
		float x, y;
		
		x = this->getPos().x;
		y = this->getPos().y;

		const CCSize& size = CCSprite::getContentSize();
		//_Trace("player ptime=%d, x=%f, y=%f,sw=%f,sh=%f", percent, x, y, size.width, size.height);
		x += CAUtils::Rand() * size.width / 3 - size.width / 6;

		//TSSpriteBubble* pspr;
		//pspr = (TSSpriteBubble*)CAWorld::sharedWorld().createSprite(_pLayer, "bubble");

		TSSpriteCommon* pspr;
		pspr = new TSSpriteCommon(_pLayer, "bubble");

		float ats = CAUtils::Rand() * 0.2f + 0.1f;
		pspr->setScl(ats / 0.5f);

		pspr->setPos(ccp(x, y));
		//pspr->setMoveDirection(-90);
		//pspr->setMoveSpeed(10);

		pspr->setZOrder(this->getZOrder() + CAUtils::Rand(-2.0f,2.0f));
		_pLayer->addSprite(pspr);

		const char* pszPose = bLow ? "pop_low" : "pop";
		pspr->setDeadPose(pszPose);
		pspr->setState(pszPose);
		
	}
}

void TSSpritePlayer::setSpeedInfo(float fPlayerSpeedInPixel, float fPlayerSpeedAcc, float fPlayerSpeedMax)
{
	_fPlayerSpeedInPixel = fPlayerSpeedInPixel;
	_fPlayerSpeedAcc = fPlayerSpeedAcc;
	_fPlayerSpeedMax = fPlayerSpeedMax;
}

void TSSpritePlayer::_on_prepare(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			this->setVisible(false);
			this->switchPose(POSE_SWIM);
			this->setPos(-5000, -5000);
			this->setMoveSpeed(0);
			//_pLayer->stage()->setOffset(ccp(0,0), 0);
		}
		break;
	case SF_Update:
		break;
	default:
		_Assert(false);
		break;
	}
}

void TSSpritePlayer::_on_dive(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			this->setPos(0, 0);
			this->setVisible(true);
			this->switchPose(POSE_BORN);

			_fPlayerSpeedLast = 0;
			_animPlayerSpeed.init(_pLayer->getTimeNow(), 0, 0, 0);
		}
		break;
	case SF_Update:
		{
			int percent = (int)(this->getAnimationPercent(_pLayer->getTimeNow()) * 100.0f);
			if (percent > 70) { percent = 70 * 2 - percent; }
			percent /= 10;
			_createBubbles(percent, false);
		}
		break;
	default:
		_Assert(false);
		break;
	}
}

//dir == +1, upward else downward
float TSSpritePlayer::_updateDirection(float dir, float a, float v0, float hs_up, float hs_down, float app_frames)
{
	CCSize size = CAWorld::getScreenSize();
	float t = _pLayer->getTimeNow() - _timeTouchEvent;
	float v1 = v0 + a * t;
	float rot = CAUtils::getRotation(-dir * v1, dir > 0 ? hs_up : hs_down);

	CCRect rect = this->getViewBoundingBox();
	if (rect.origin.y < _rectYard.origin.y && rot > 0)
	{
		rot = 0;
	}
	else if (rect.origin.y + rect.size.height > _rectYard.origin.y + _rectYard.size.height
		&& rot < 0)
	{
		rot = 0;
	}

	_toRotation = rot;

	rot = this->getMoveDirection();
	float rot_frames = app_frames; //_settings().getFloat("rot_frames");
	//float rot_min_delta = _settings().getFloat("rot_min_delta");
	float delta = (_toRotation - rot) / rot_frames;
	if (_Abs(rot - _toRotation) < _Abs(delta))
	{
		rot = _toRotation;
	}
	else
	{
		rot += delta;
	}
	//this->setMoveDirection(rot);
	return rot;
}

#define INDEX_PLAYER	0
#define INDEX_DOLPHIN	1
#define INDEX_WHALE		2
float TSSpritePlayer::_updateSwimDirection(float dir)
{
	return _updateDirection(dir, _swim_a[INDEX_PLAYER], _swim_v0[INDEX_PLAYER], _swim_hs_up[INDEX_PLAYER], _swim_hs_down[INDEX_PLAYER], _swim_app_frames[INDEX_PLAYER]);
}

float TSSpritePlayer::_updateDolphinDirection(float dir)
{
	return _updateDirection(dir, _swim_a[INDEX_DOLPHIN], _swim_v0[INDEX_DOLPHIN], _swim_hs_up[INDEX_DOLPHIN], _swim_hs_down[INDEX_DOLPHIN], _swim_app_frames[INDEX_DOLPHIN]);
}

float TSSpritePlayer::_updateWhaleDirection(float dir)
{
	return _updateDirection(dir, _swim_a[INDEX_WHALE], _swim_v0[INDEX_WHALE], _swim_hs_up[INDEX_WHALE], _swim_hs_down[INDEX_WHALE], _swim_app_frames[INDEX_WHALE]);
}

void TSSpritePlayer::setDistance4CalculatingSpeed(float distance)
{
	float speed = _fPlayerSpeedInPixel + _fPlayerSpeedAcc * distance;
	if (speed > _fPlayerSpeedMax) speed = _fPlayerSpeedMax;

	_animPlayerSpeed.init(_pLayer->getTimeNow(), 0, _fPlayerSpeedLast, speed);
	_fPlayerSpeedLast = speed;
}

void TSSpritePlayer::_on_swiming(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			this->setPos(ccp(
				this->getCombinedKey().x, this->getCombinedKey().y));
			this->switchPose(POSE_SWIM);
			this->setMoveDirection(0);
		}
		break;
	case SF_Update:
		{
			if (_animPlayerSpeed.isValid())
			{
				this->setMoveSpeed(_animPlayerSpeed.getValue(_pLayer->getTimeNow()));
			}

			int n = (int)(_pLayer->getTimeNow() * 1000);
			n %= 1000;
			if (n < 100)
			{
				_createBubbles(1, true);
			}
			float rot = _updateSwimDirection(_direction);
			this->setMoveDirection(rot);
		}
		break;
	default:
		_Assert(false);
		break;
	}
}

void TSSpritePlayer::_on_hurt(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			this->setPos(ccp(
				this->getCombinedKey().x, this->getCombinedKey().y));
			this->switchPose(POSE_HURT);
			//this->setMoveDirection(0);
			if (null != _psprRider)
			{
				//_psprRider->killMyself();
				_psprRider->setMoveSpeed(_fPlayerSpeedLast * 0.8f);
				_psprRider = null;
			}
		}
		break;
	case SF_Update:
		{
			if (this->isAnimationDone())
			{
				setState(PS_Swiming);
			}
			float rot = _updateSwimDirection(_direction);
			this->setMoveDirection(rot);
		}
		break;
	default:
		_Assert(false);
		break;
	}
}

void TSSpritePlayer::_on_riding_dolphin(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			this->setPos(ccp(this->getCombinedKey().x, this->getCombinedKey().y));
			this->switchPose(POSE_RIDING_DOLPHIN);
			this->setMoveDirection(0);
			//create some bubbles
			_createBubbles(5, true);
			//create rider
			_psprRider = new TSSpriteCommon(_pLayer, "dolphin");
			_psprRider->setPos(this->getPos() + _posOffsetDolphin);
			_psprRider->setState("swim");
			_pLayer->addSprite(_psprRider);

			float speed = this->getMoveSpeed();
			speed *= _speed_weight_dolphin;
			this->setMoveSpeed(speed);
		}
		break;
	case SF_Update:
		{
			int n = (int)(_pLayer->getTimeNow() * 1000);
			n %= 1000;
			if (n < 100)
			{
				_createBubbles(1, true);
			}

			_Assert(_psprRider);
			_psprRider->setPos(this->getPos() + _posOffsetDolphin);
			float rot = _updateDolphinDirection(_direction);
			this->setMoveDirection(rot);
			_psprRider->setMoveDirection(rot);
		}
		break;
	default:
		_Assert(false);
		break;
	}
}

void TSSpritePlayer::_on_riding_whale(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			this->setPos(ccp(this->getCombinedKey().x, this->getCombinedKey().y));
			this->switchPose(POSE_RIDING_WHALE);
			this->setMoveDirection(0);
			//create some bubbles
			_createBubbles(5, true);
			//create rider
			_psprRider = new TSSpriteCommon(_pLayer, "whale");
			_psprRider->setPos(this->getPos() + _posOffsetWhale);
			_psprRider->setState("swim");
			_pLayer->addSprite(_psprRider);

			float speed = this->getMoveSpeed();
			speed *= _speed_weight_whale;
			this->setMoveSpeed(speed);
		}
		break;
	case SF_Update:
		{
			int n = (int)(_pLayer->getTimeNow() * 1000);
			n %= 1000;
			if (n < 100)
			{
				_createBubbles(1, true);
			}

			_Assert(_psprRider);
			_psprRider->setPos(this->getPos() + _posOffsetWhale);
			float rot = _updateWhaleDirection(_direction);
			this->setMoveDirection(rot);
			_psprRider->setMoveDirection(rot);
		}
		break;
	default:
		_Assert(false);
		break;
	}
}

void TSSpritePlayer::_on_dead(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			this->switchPose(POSE_DEAD);
			this->setMoveDirection(0);
			//this->setMoveSpeed(0);
		}
		break;
	case SF_Update:
		{
		}
		break;
	default:
		_Assert(false);
		break;
	}
}

void TSSpritePlayer::_on_fadeout(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			this->switchPose(PS_Fadeout);
		}
		break;
	case SF_Update:
		{
		}
		break;
	default:
		_Assert(false);
		break;
	}
}

void TSSpritePlayer::onStateChanged(const string& olds, const string& news)
{
	_HandleState(news, prepare, SF_Begin);
	_HandleState(news, dive, SF_Begin);
	_HandleState(news, swiming, SF_Begin);
	_HandleState(news, hurt, SF_Begin);
	_HandleState(news, riding_dolphin, SF_Begin);
	_HandleState(news, riding_whale, SF_Begin);
	_HandleState(news, dead, SF_Begin);
	_HandleState(news, fadeout, SF_Begin);
}

void TSSpritePlayer::_adjustPosition(CCPoint& pos)
{
	//_Trace("player[%s] gx=%.0f,gy=%.0f cx=%.2f cy=%.0f", _state.c_str(),
	//	_gameKey.x, _gameKey.y, _combinedKey.x, _combinedKey.y);
	if (this->getCurrentPose() && this->getCurrentPose()->name() == "fadeout")
		return;

	if (pos.y < _rectYard.origin.y)
	{
		pos.y = _rectYard.origin.y;
	}
	else if (pos.y > _rectYard.origin.y + _rectYard.size.height)
	{
		pos.y = _rectYard.origin.y + _rectYard.size.height;
	}
}

void TSSpritePlayer::onUpdate()
{
	CASprite::onUpdate();

	_HandleState(_state, prepare, SF_Update);
	_HandleState(_state, dive, SF_Update);
	_HandleState(_state, swiming, SF_Update);
	_HandleState(_state, hurt, SF_Update);
	_HandleState(_state, riding_dolphin, SF_Update);
	_HandleState(_state, riding_whale, SF_Update);
	_HandleState(_state, dead, SF_Update);
}

bool TSSpritePlayer::onEvent(CAEvent* pEvent)
{
	switch (pEvent->type())
	{
	case ET_Touch:
		{
			CAEventTouch* pe = (CAEventTouch*)pEvent;

			if (_state == PS_Prepare)
			{
				this->setPos(0, 0);
				setState(PS_Dive);
			}
			else
			{
				if (pe->state() == kTouchStateGrabbed)
				{
					_direction = +1;
					_timeTouchEvent = _pLayer->getTimeNow();
				}
				else if (pe->state() == kTouchStateUngrabbed)
				{
					_direction = -1;
					_timeTouchEvent = _pLayer->getTimeNow();
				}
			}
			return true;
		}
		break;
	}
	return false;
}

void TSSpritePlayer::ride(const string& rider)
{
	if (rider == "dolphin")
	{
		setState(PS_RidingDolphin);
	}
	else if (rider == "whale")
	{
		setState(PS_RidingWhale);
	}
	else
	{
		_Assert(false);
	}
}

string TSSpritePlayer::debuglog() const
{
	char sz[256];
	float t = _pLayer->getTimeNow();
	sprintf(sz, "%s,%.2f=%.2f-%.2f ", 
		_direction > 0 ? "U" : "D", 
		t - _timeTouchEvent, t, _timeTouchEvent);
	string r = sz;
	return r;
}

