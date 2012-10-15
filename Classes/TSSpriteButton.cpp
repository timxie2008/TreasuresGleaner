
#include "TSSpriteButton.h"
#include "AStageLayer.h"
#include "AStage.h"

#define ButtonPose_Stand	"stand"
#define ButtonPose_Pressed	"pressed"
#define ButtonPose_DeadPose	"deadpose"

TSSpriteButton::TSSpriteButton(CAStageLayer* player, const char* name) : CASprite(player, name)
{
	_nClickState = 0;
}

TSSpriteButton::~TSSpriteButton(void)
{
}

void TSSpriteButton::_on_state_event(EStateFlag flag)
{
	switch(flag)
	{
	case SF_Begin:
		{
			if (_model->hasPose(_state))
			{
				this->switchPose(_state);
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

void TSSpriteButton::_onAnimationStop()
{
	CASpriteModelPose* ppose = this->getCurrentPose();
	if (ppose->name() == ButtonPose_Pressed)
	{
		setState(ButtonPose_Stand);
	}
	if (_settings().hasKey(ButtonPose_DeadPose))
	{
		string dp = _settings().getString(ButtonPose_DeadPose);
		CASpriteModelPose* ppose = this->_currentPose;
		if (_state == dp && ppose->name() == dp)
		{
			this->killMyself();
		}
	}
}

void TSSpriteButton::onStateChanged(const string& olds, const string& news)
{
	_on_state_event(SF_Begin);
}

void TSSpriteButton::onEnter()
{
	this->setTouchable(true);
	this->setControlOrder(1);
	_pLayer->addControl(this);
	CASprite::onEnter();
}

void TSSpriteButton::onUpdate()
{
	CASprite::onUpdate();
	_on_state_event(SF_Update);
}

void TSSpriteButton::onExit()
{
	CASprite::onExit();
}

void TSSpriteButton::onTouchLeave(CAEventTouch* pEvent) 
{
	_nClickState = 0;
	setState(ButtonPose_Stand);
}

void TSSpriteButton::onTouched(CAEventTouch* pEvent) 
{
	GUARD_FUNCTION();

	switch (pEvent->state())
	{
	case kTouchStateGrabbed:
		_nClickState = 1;
		setState(ButtonPose_Pressed);
		this->layer()->stage()->playEffect("button_click");
		//_Trace("button %s pressed", this->getModName().c_str());
		break;
	case kTouchStateUngrabbed:
		//_Trace("button %s state=%d", this->getModName().c_str(), _nClickState);
		if (_nClickState == 1) //_state == ButtonPose_Pressed && 
		{
			GUARD_FIELD(_button_touch_event);
			//_Trace("button %s clicked", this->getModName().c_str());
			_nClickState = 2;
			_pLayer->onEvent(new CAEventCommand(this, "onClick"));
			//setState(ButtonPose_Stand);
		}
		break;
	}
}
