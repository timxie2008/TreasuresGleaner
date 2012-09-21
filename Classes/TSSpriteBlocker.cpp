
#include "TSSpriteBlocker.h"

TSSpriteBlocker::TSSpriteBlocker(CAStageLayer* player, const char* name) : CASprite(player, name)
{
}

TSSpriteBlocker::~TSSpriteBlocker(void)
{
}

void TSSpriteBlocker::_on_state_event(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			this->switchPose(_state);
			if (_state == "dismiss")
			{
				//do some blooommmmm
			}
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

void TSSpriteBlocker::_onAnimationStop()
{
	string dp = _overrided_deadpose;
	if (dp.length() <= 0)
	{
		if (_settings().hasKey("deadpose"))
		{
			dp = _settings().getString("deadpose");
		}
	}
	if (dp.length() > 0)
	{
		CASpriteModelPose* ppose = this->_currentPose;
		if (_state == dp && ppose->name() == dp)
		{
			this->killMyself();
		}
	}
}

void TSSpriteBlocker::onStateChanged(const string& olds, const string& news)
{
	_on_state_event(SF_Begin);
}

void TSSpriteBlocker::onUpdate()
{
	CASprite::onUpdate();
	_on_state_event(SF_Update);
}

