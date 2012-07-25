
#include "TSSpriteBubble.h"

TSSpriteBubble::TSSpriteBubble(CAStageLayer* player, const char* name) : CASprite(player, name)
{
}

TSSpriteBubble::~TSSpriteBubble(void)
{
}

void TSSpriteBubble::_onAnimationStop()
{
	this->killMyself();
}

void TSSpriteBubble::_on_pop(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			this->switchPose("pop");
		}
		break;
	case SF_Update:
		break;
	default:
		_Assert(false);
		break;
	}
}

void TSSpriteBubble::onStateChanged(const string& olds, const string& news)
{
	_HandleState(news, pop, SF_Begin);
}

