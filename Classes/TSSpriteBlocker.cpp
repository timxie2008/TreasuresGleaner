
#include "AWorld.h"
#include "AStageLayer.h"
#include "TSSpriteBlocker.h"
#include "TSSpriteCommon.h"

TSSpriteBlocker::TSSpriteBlocker(CAStageLayer* player, const char* name) : CASprite(player, name)
{
}

TSSpriteBlocker::~TSSpriteBlocker(void)
{
}

TSSpriteCommon* TSSpriteBlocker::_createCommonSprite(const char* name, const char* state, const CCPoint& pos, bool autoremove, float scale)
{
	TSSpriteCommon* pspr = new TSSpriteCommon(_pLayer, name);
	pspr->setPos(pos);
	pspr->setScl(scale);
	pspr->setState(state);
	if (autoremove)
	{
		pspr->setDeadPose(state);
	}
	return pspr;
}

void TSSpriteBlocker::_on_state_event(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			CCSize size = CAWorld::getScreenSize();
			this->switchPose(_state);
			if (_state == "dismiss")
			{
				//do some blooommmmm
				TSSpriteCommon* pspr;
				pspr = _createCommonSprite("cloud", "cloud_02_01", this->getPos(), true, 1.2f);
				_pLayer->addSprite(pspr);
				pspr = _createCommonSprite("cloud", "cloud_01_01", this->getPos(), true, 1.1f);
				_pLayer->addSprite(pspr);

				int i, c;
				c = 64;
				for (i = 0; i < c; i++)
				{
					char szPose[32];
					sprintf(szPose, "slice_%d", (i % 4) + 1);
					CCPoint pos;
					pos = this->getPos();

					float ms = (CAUtils::Rand() * 80.0f + 60.0f) * size.width / 100.0f * 2.0f;
					float dir = CAUtils::Rand() * 360.0f;
					pspr = _createCommonSprite("blocker-01", szPose, pos, true);
					pspr->setGroupName("noname");
					pspr->setMoveDirection(dir);
					pspr->setMoveSpeed(ms);
					_pLayer->addSprite(pspr);
				}
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

