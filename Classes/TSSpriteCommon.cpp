
#include "TSSpriteCommon.h"

TSSpriteCommon::TSSpriteCommon(CAStageLayer* player, const char* name) : CASprite(player, name)
{
}

TSSpriteCommon::~TSSpriteCommon(void)
{
}

void TSSpriteCommon::_on_state_event(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			this->switchPose(_state);
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

void TSSpriteCommon::_onAnimationStop()
{
	if (_settings().hasKey("deadpose"))
	{
		string dp = _settings().getString("deadpose");
		if (dp.size() > 0)
		{
			CASpriteModelPose* ppose = this->_currentPose;
			if (_state == dp && ppose->name() == dp)
			{
				this->killMyself();
			}
		}
	}
}

void TSSpriteCommon::onStateChanged(const string& olds, const string& news)
{
	_on_state_event(SF_Begin);
}

void TSSpriteCommon::onUpdate()
{
	CASprite::onUpdate();
	_on_state_event(SF_Update);
}

