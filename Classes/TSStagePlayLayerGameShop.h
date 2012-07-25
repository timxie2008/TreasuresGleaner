#if !defined(_STAGE_PLAY_LAYER_GAME_SHOP_H_)
#define _STAGE_PLAY_LAYER_GAME_SHOP_H_

#include "AStageLayer.h"

class TSSpritePlayer;
class TSSpriteCommon;
class TSSpriteButton;

class TSStagePlayLayerGameShop :
	public CAStageLayer //, public CCTargetedTouchDelegate
{
protected:
	Define_Sprite_Getter(TSSpriteCommon, label_pause_title);
	Define_Sprite_Getter(TSSpriteCommon, panel_back);
	Define_Sprite_Getter(TSSpriteButton, button_restart);
	Define_Sprite_Getter(TSSpriteButton, button_resume);
	Define_Sprite_Getter(TSSpriteButton, button_music);
	Define_Sprite_Getter(TSSpriteButton, button_sound);

	void _updateNumber(const char* prefix, int nValue);

	DELCARE_STATE_HANDLER(Idle);
	DELCARE_STATE_HANDLER(Fadein);
	DELCARE_STATE_HANDLER(Running);
	DELCARE_STATE_HANDLER(OnResume);
	DELCARE_STATE_HANDLER(OnRestart);
	DELCARE_STATE_HANDLER(Fadeout);
public:
	TSStagePlayLayerGameShop(CAStage* pstage, CAStageLayer* playerParent);
	virtual ~TSStagePlayLayerGameShop(void);

	virtual void show(bool s);
	virtual string debuglog();

	virtual void onEnter();
	virtual void onUpdate();
	virtual void onExit();

	virtual void onEvent(CAEvent* pevt);

	virtual bool checkCondition(CAState* from, const CATransition& trans);

	virtual void onStateBegin(CAState* from, void* param);
	//virtual void onStateUpdate(CAState* from, size_t counter, ccTime time);
	virtual void onStateEnd(CAState* from, void* param);
};

#endif //_STAGE_PLAY_LAYER_GAME_SHOP_H_
