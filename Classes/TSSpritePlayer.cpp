
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

	_bJumping = false;

	_fPlayerHSpeedLast = 0;
	_vSpeedWhenTouched = 0;

	_fPlayerSpeedInPixel = 0;
	_fPlayerSpeedAcc = 0;
	_fPlayerSpeedMax = 0;
	_fPlayerSpeedJumpPower = 0;

	_animPlayerSpeed.setValid(false);

	_direction = 0;
	//_toRotation = 0;
	_timeTouchEvent = 0;
	_rectYard = _settings().getRect("yard");
	CAWorld::percent2view(_rectYard);

	int i;
	char* szNames[] = { "player", "dolphin", "whale" };
	for (i = 0; i < 3; i++)
	{
		_TPlayerSpeedParams& sp = _speed_params[i];

		string name;

		name = szNames[i];	name += "_touch_power";	sp.touch_power = 1.0f - _settings().getFloat(name.c_str());			
		name = szNames[i];	name += "_climbing_a";	sp.climbing_a = _settings().getFloat(name.c_str());			CAWorld::percent2view(sp.climbing_a, false);
		name = szNames[i];	name += "_v0";			sp.v0 = _settings().getFloat(name.c_str());					CAWorld::percent2view(sp.v0, false);
		name = szNames[i];	name += "_v1";			sp.v1 = _settings().getFloat(name.c_str());					CAWorld::percent2view(sp.v1, false);
		name = szNames[i];	name += "_hs_up";		sp.hs_up = _settings().getFloat(name.c_str());				CAWorld::percent2view(sp.hs_up, false);
		name = szNames[i];	name += "_hs_down";		sp.hs_down = _settings().getFloat(name.c_str());			CAWorld::percent2view(sp.hs_down, false);
		name = szNames[i];	name += "_app_frames";	sp.app_frames = _settings().getFloat(name.c_str());
	}
	_falling_a = _settings().getFloat("falling_a", 1.42f);	CAWorld::percent2view(_falling_a, false);
	_climbing_v = _settings().getFloat("climbing_v", 1.42f);	CAWorld::percent2view(_climbing_v, false);

	_speed_weight_dolphin = _settings().getFloat("speed_weight_dolphin");
	_speed_weight_whale = _settings().getFloat("speed_weight_whale");

	_posOffsetDolphin = _settings().getPoint("offset_dolphin");
	CAWorld::percent2view(_posOffsetDolphin);
	_posOffsetWhale = _settings().getPoint("offset_whale");
	CAWorld::percent2view(_posOffsetWhale);

	_fPlayerSpeedInPixel = _settings().getFloat("player_speed_in_percent");
	CAWorld::percent2view(_fPlayerSpeedInPixel, true);
	_fPlayerSpeedAcc = _settings().getFloat("player_speed_acc_in_distance");
	_fPlayerSpeedMax = _settings().getFloat("player_speed_max_in_percent");
	CAWorld::percent2view(_fPlayerSpeedMax, false);

	_fPlayerSpeedJumpPower = _settings().getFloat("player_speed_jump_power");
	CAWorld::percent2view(_fPlayerSpeedJumpPower, true);

	this->setFreeSpeedMode();

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

bool TSSpritePlayer::isCollidWith(CASprite* pspr, bool bView)
{
	if (CASprite::isCollidWith(pspr, bView))
		return true;
	if (false && null != _psprRider)
	{
		return _psprRider->isCollidWith(pspr, bView);
	}
	return false;
}

void TSSpritePlayer::_onAnimationStart()
{
	//_toRotation = 0;
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
		this->setHMoveSpeed(0);
		this->setVMoveSpeed(0);
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

		pspr->setZOrder(this->getZOrder() + CAUtils::Rand(-2.0f,2.0f));
		_pLayer->addSprite(pspr);

		const char* pszPose = bLow ? "pop_low" : "pop";
		pspr->setDeadPose(pszPose);
		pspr->setState(pszPose);
		
	}
}

#if 0
void TSSpritePlayer::setSpeedInfo(float fPlayerSpeedInPixel, float fPlayerSpeedAcc, float fPlayerSpeedMax, float fPlayerSpeedJumpPower)
{
	_fPlayerSpeedJumpPower = fPlayerSpeedJumpPower;
	_fPlayerSpeedInPixel = fPlayerSpeedInPixel;
	_fPlayerSpeedAcc = fPlayerSpeedAcc;
	_fPlayerSpeedMax = fPlayerSpeedMax;
}
#endif

void TSSpritePlayer::_on_prepare(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			this->setVisible(false);
			this->switchPose(POSE_SWIM);
			this->setPos(-5000, -5000);
			this->setHMoveSpeed(0);
			this->setVMoveSpeed(0);
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

			_fPlayerHSpeedLast = 0;
			_vSpeedWhenTouched = 0;
			_animPlayerSpeed.init(_pLayer->getTimeNow(), 0, 0, 0);

			this->_pLayer->stage()->playEffect("player_dive");
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
float TSSpritePlayer::_updateVSpeedAndRotation(int index)
{
	_Assert(index >= 0 && index < 3);

	float t = _pLayer->getTimeNow() - _timeTouchEvent;

	const _TPlayerSpeedParams& sp = _speed_params[index];
	float vSpeed;
	
	float a;
	if (_direction < 0)
	{
		//falling
		a = -_falling_a;
	}
	else
	{
		//jumping
		if (_bJumping)
		{
			a = -_falling_a;
		}
		else
		{
			a = sp.climbing_a;
		}
	}
	vSpeed = _vSpeedWhenTouched * sp.touch_power + a * t;
	if (_bJumping)
	{
		if (vSpeed < _climbing_v)
		{
			_Info("jump finished");
			_vSpeedWhenTouched = _climbing_v;
			_bJumping = false;
		}
	}
	CCPoint pos = this->getPos();
	if (CAUtils::almostEqual(pos.y, _rectYard.origin.y, 0.001f) && vSpeed < 0)
	{
		vSpeed = 0;
	}
	else if (CAUtils::almostEqual(pos.y, _rectYard.origin.y + _rectYard.size.height, 0.001f) && vSpeed > 0)
	{
		vSpeed = 0;
	}

	this->setVMoveSpeed(vSpeed);

	float rotOld = this->getMoveDirection();
	float rotNew = CAUtils::getRotation(-vSpeed, _direction > 0 ? sp.hs_up : sp.hs_down);

	float app_frames = sp.app_frames;
	float delta = (rotNew - rotOld) / app_frames;
	if (_Abs(rotNew - rotOld) > _Abs(delta))
	{
		rotNew = rotOld + delta;
	}
	this->setMoveDirection(rotNew);
	
	return rotNew;
}

void TSSpritePlayer::setDistance4CalculatingHSpeed(float distance)
{
	float speed = _fPlayerSpeedInPixel + _fPlayerSpeedAcc * distance;
	if (speed > _fPlayerSpeedMax) speed = _fPlayerSpeedMax;

	_animPlayerSpeed.init(_pLayer->getTimeNow(), 0, _fPlayerHSpeedLast, speed);
	_fPlayerHSpeedLast = speed;
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
				//this->setMoveSpeed(_animPlayerSpeed.getValue(_pLayer->getTimeNow()));
				this->setHMoveSpeed(_animPlayerSpeed.getValue(_pLayer->getTimeNow()));
			}

			int n = (int)(_pLayer->getTimeNow() * 1000);
			n %= 1000;
			if (n < 100)
			{
				_createBubbles(1, true);
			}
			_updateSwim();
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
			_createBubbles(5, true);
			if (null != _psprRider)
			{
				//_psprRider->killMyself();
				//follow the player speed and rotation
				_psprRider->setMoveSpeed(_fPlayerHSpeedLast * 0.7f);
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
			_updateSwim();
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

			this->_pLayer->stage()->playEffect("ride_dolphin");

			//this->setMoveDirection(0);
			//create some bubbles
			_createBubbles(5, true);
			//create rider
			_psprRider = new TSSpriteCommon(_pLayer, "dolphin");
			_psprRider->setPos(this->getPos() + _posOffsetDolphin);
			_psprRider->setVertexZ(this->getVertexZ());
			_psprRider->setState("swim");
			_pLayer->addSprite(_psprRider);

			float speed = this->getHMoveSpeed();
			speed *= _speed_weight_dolphin;
			this->setHMoveSpeed(speed);
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

			float rot = _updateDolphin();
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

			this->_pLayer->stage()->playEffect("ride_whale");

			//create some bubbles
			_createBubbles(5, true);
			//create rider
			_psprRider = new TSSpriteCommon(_pLayer, "whale");
			_psprRider->setPos(this->getPos() + _posOffsetWhale);
			_psprRider->setVertexZ(this->getVertexZ());
			_psprRider->setState("swim");
			_pLayer->addSprite(_psprRider);

			float speed = this->getHMoveSpeed();
			speed *= _speed_weight_whale;
			this->setHMoveSpeed(speed);
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

			float rot = _updateWhale();
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
			this->_pLayer->stage()->playEffect("player_die");
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

void TSSpritePlayer::_on_floating(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			this->switchPose(PS_Floating);
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
	else _HandleState(news, dive, SF_Begin);
	else _HandleState(news, swiming, SF_Begin);
	else _HandleState(news, hurt, SF_Begin);
	else _HandleState(news, riding_dolphin, SF_Begin);
	else _HandleState(news, riding_whale, SF_Begin);
	else _HandleState(news, dead, SF_Begin);
	else _HandleState(news, fadeout, SF_Begin);
	else _HandleState(news, floating, SF_Begin);
}

void TSSpritePlayer::_adjustGamePosition(float& x, float& y)
{
	CCPoint pos = ccp(x, y);
	_adjustPosition(pos);
	x = pos.x;
	y = pos.y;
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

bool TSSpritePlayer::_isRunningOnGround() const
{
	const CCPoint& pos = this->getPos();
	if (CAUtils::almostEqual(pos.y, _rectYard.origin.y, 0.01f))
		return true;
	return false;
}

bool TSSpritePlayer::_isRunningOnCeil() const
{
	const CCPoint& pos = this->getPos();
	if (CAUtils::almostEqual(pos.y, _rectYard.origin.y + _rectYard.size.height, 0.01f))
		return true;
	return false;
}

void TSSpritePlayer::onUpdate()
{
	CASprite::onUpdate();

	_HandleState(_state, prepare, SF_Update);
	else _HandleState(_state, dive, SF_Update);
	else _HandleState(_state, swiming, SF_Update);
	else _HandleState(_state, hurt, SF_Update);
	else _HandleState(_state, riding_dolphin, SF_Update);
	else _HandleState(_state, riding_whale, SF_Update);
	else _HandleState(_state, dead, SF_Update);
	else _HandleState(_state, floating, SF_Update);
}

void TSSpritePlayer::pause()
{
}

void TSSpritePlayer::resume(float paused)
{
	_timeTouchEvent += paused;
	_onTouchFalling();
}

void TSSpritePlayer::_onTouchClimbing()
{
	_direction = +1;
	_createBubbles(5, true);
	if (_isRunningOnGround() && !this->isRidding())
	{
		_Info("jump !!!");
		this->setVMoveSpeed(_fPlayerSpeedJumpPower);
		_bJumping = true;
		//_vSpeedWhenTouched = _speed_params[0].v0;
	}
	_vSpeedWhenTouched = this->getVMoveSpeed();

	_timeTouchEvent = _pLayer->getTimeNow();

	_pLayer->stage()->playEffect("pop_bubble");
}

void TSSpritePlayer::_onTouchFalling()
{
	_direction = -1;
	_vSpeedWhenTouched = this->getVMoveSpeed();
	if (!this->isRidding() && _isRunningOnCeil())
	{
		_vSpeedWhenTouched = _speed_params[INDEX_PLAYER].v0;
	}
	_timeTouchEvent = _pLayer->getTimeNow();
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
					_onTouchClimbing();
				}
				else if (pe->state() == kTouchStateUngrabbed)
				{
					_onTouchFalling();
				}
				else
				{
					//move
					//DO NOT recored _timeTouchEvent
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
	sprintf(sz, "%s,VS=%.2f ", _direction > 0 ? "U" : "D", this->getVMoveSpeed());
	string r = sz;
	return r;
}

