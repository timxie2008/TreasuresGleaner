
#include "TSSpriteFish.h"
#include "AStage.h"
#include "AStageLayer.h"

TSSpriteFish::TSSpriteFish(CAStageLayer* player, const char* name) : CASprite(player, name)
{
}

TSSpriteFish::~TSSpriteFish(void)
{
}

void TSSpriteFish::_on_swim(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			this->switchPose("swim");	
		}
		break;
	case SF_Update:
		{
			CCRect r = this->getGameBoundingBox();
			if (r.origin.x + r.size.width < _pLayer->stage()->offset().x)
			{
				this->killMyself();
			}
		}
		break;
	default:
		_Assert(false);
		break;
	}
}

void TSSpriteFish::onStateChanged(const string& olds, const string& news)
{
	_HandleState(news, swim, SF_Begin);
}

void TSSpriteFish::onUpdate()
{
	CASprite::onUpdate();
	_HandleState(_state, swim, SF_Update);
}

