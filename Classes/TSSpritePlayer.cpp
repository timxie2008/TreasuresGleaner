
#include "TSSpritePlayer.h"
#include "TSSpriteBubble.h"
#include "AWorld.h"
#include "AStage.h"
#include "AStageLayer.h"

static const char* POSE_BORN = "born";
static const char* POSE_SWIM = "swim";
static const char* POSE_DEAD = "dead";

TSSpritePlayer::TSSpritePlayer(CAStageLayer* player, const char* name) : CASprite(player, name)
{
	_Trace("player created");
	_direction = 0;
	_toRotation = 0;
	_timeTouchEvent = 0;
	_rectYard = _settings().getRect("yard");
	CAWorld::percent2view(_rectYard);

	//this->setZOrder(_settings().getFloat("player_z"));

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

		TSSpriteBubble* pspr;
		pspr = (TSSpriteBubble*)CAWorld::sharedWorld().createSprite(_pLayer, "bubble");

		float ats = CAUtils::Rand() * 0.2f + 0.1f;
		pspr->setScl(ats / 0.5f);

		pspr->setPos(ccp(x, y));
		//pspr->setMoveDirection(-90);
		//pspr->setMoveSpeed(10);

		pspr->setZOrder(this->getZOrder() + CAUtils::Rand(-2.0f,2.0f));
		_pLayer->addSprite(pspr);

		pspr->switchPose(bLow ? "pop_low" : "pop");
		
	}
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
void TSSpritePlayer::_updateDirection(float dir)
{
	CCSize size = CAWorld::getScreenSize();

	float a = _settings().getFloat("rot_rat");
	float b = 0.0f;
	float x = _pLayer->getTimeNow() - _timeTouchEvent;
	float time = x;
	float y = a * (x * x) + b;
	float speed = this->getMoveSpeed();

	//float vspeed = ac * time * size.width;
	//float hspeed = speed;
	float vspeed = y;
	float hspeed = x;

	float rot_max_tan;
	if (dir > 0) //upward
		rot_max_tan = _settings().getFloat("rot_max_up");
	else
		rot_max_tan = _settings().getFloat("rot_max_down");
	if (vspeed / hspeed > rot_max_tan)
		vspeed = rot_max_tan * hspeed;

	float rot = CAUtils::getRotation(-dir * vspeed, hspeed);

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
	float rot_frames = _settings().getFloat("rot_frames");
	float rot_min_delta = _settings().getFloat("rot_min_delta");
	float delta = (_toRotation - rot) / rot_frames;
	if (abs(rot - _toRotation) < abs(delta))
	{
		rot = _toRotation;
	}
	else
	{
		rot += delta;
	}
	this->setMoveDirection(rot);
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
#if defined(_DEBUG) || 1
		{
			int n = (int)(_pLayer->getTimeNow() * 1000);
			n %= 1000;
			if (n < 100)
			{
				_createBubbles(1, true);
			}
		}
#endif
		_updateDirection(_direction);
		break;
	default:
		_Assert(false);
		break;
	}
}

void TSSpritePlayer::_on_waitriding(EStateFlag flag)
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
#if defined(_DEBUG) || 1
		{
			int n = (int)(_pLayer->getTimeNow() * 1000);
			n %= 1000;
			if (n < 100)
			{
				_createBubbles(1, true);
			}
		}
#endif
		_updateDirection(_direction);
		break;
	default:
		_Assert(false);
		break;
	}
}

void TSSpritePlayer::_on_riding(EStateFlag flag)
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
#if defined(_DEBUG) || 1
		{
			int n = (int)(_pLayer->getTimeNow() * 1000);
			n %= 1000;
			if (n < 100)
			{
				_createBubbles(1, true);
			}
		}
#endif
		_updateDirection(_direction);
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

void TSSpritePlayer::onStateChanged(const string& olds, const string& news)
{
	_HandleState(news, prepare, SF_Begin);
	_HandleState(news, dive, SF_Begin);
	_HandleState(news, swiming, SF_Begin);
	_HandleState(news, waitriding, SF_Begin);
	_HandleState(news, riding, SF_Begin);
	_HandleState(news, dead, SF_Begin);
}

void TSSpritePlayer::_adjustPosition(CCPoint& pos)
{
	//_Trace("player[%s] gx=%.0f,gy=%.0f cx=%.2f cy=%.0f", _state.c_str(),
	//	_gameKey.x, _gameKey.y, _combinedKey.x, _combinedKey.y);

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
	_HandleState(_state, waitriding, SF_Update);
	_HandleState(_state, riding, SF_Update);
	_HandleState(_state, dead, SF_Update);
}

/*
void TSSpritePlayer::setMoveSpeed(float s)
{
	if (_state == PS_Swiming)
	{
		CASprite::setMoveSpeed(s);
	}
	else
	{
		CASprite::setMoveSpeed(0);
	}
}
*/

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
				//this->resetTimer();
			}
			return true;
		}
		break;
	}
	return false;
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

