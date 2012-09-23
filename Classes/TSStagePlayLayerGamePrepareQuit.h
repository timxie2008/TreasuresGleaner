#if !defined(_STAGE_PLAY_LAYER_GAME_PREPARE_QUIT_H_)
#define _STAGE_PLAY_LAYER_GAME_PREPARE_QUIT_H_

#include "AStageLayer.h"

class TSSpritePlayer;
class TSSpriteCommon;
class TSSpriteButton;

#define EVENT_ONRESUME	"onResume"
#define EVENT_ONRESTART	"onRestart"

class TSStagePlayLayerGamePrepareQuit :
	public CAStageLayer //, public CCTargetedTouchDelegate
{
protected:
	Define_Sprite_Getter(TSSpriteCommon, panel_black);
	Define_Sprite_Getter(TSSpriteCommon, panel_quit);
	Define_Sprite_Getter(TSSpriteButton, button_yes);
	Define_Sprite_Getter(TSSpriteButton, button_no);

	void _NullGetters()
	{
		Init_Sprite_Getter_Var(panel_black);
		Init_Sprite_Getter_Var(panel_quit);
		Init_Sprite_Getter_Var(button_yes);
		Init_Sprite_Getter_Var(button_no);
	}

	void _InitGetters()
	{
		Init_Sprite_Getter(TSSpriteCommon, panel_black);
		Init_Sprite_Getter(TSSpriteCommon, panel_quit);
		Init_Sprite_Getter(TSSpriteButton, button_yes);
		Init_Sprite_Getter(TSSpriteButton, button_no);
	}

	void _updateNumber(const char* prefix, int nValue);
public:
	TSStagePlayLayerGamePrepareQuit(CAStage* pstage, CAStageLayer* playerParent);
	virtual ~TSStagePlayLayerGamePrepareQuit(void);

	virtual void show(bool s);

	virtual void onEnter();
	virtual void onUpdate();
	virtual void onExit();

	virtual void onEvent(CAEvent* pevt);

	virtual bool checkCondition(CAState* from, const CATransition& trans);

	virtual void onStateBegin(CAState* from, void* param);
	//virtual void onStateUpdate(CAState* from, size_t counter, ccTime time);
	virtual void onStateEnd(CAState* from, void* param);
};

#endif //_STAGE_PLAY_LAYER_GAME_PREPARE_QUIT_H_
