#if !defined(_STAGE_PLAY_LAYER_GAME_PREPARE_H_)
#define _STAGE_PLAY_LAYER_GAME_PREPARE_H_

#include "AStageLayer.h"
#include "TSGroupNumber.h"

class TSSpritePlayer;
class TSSpriteCommon;
class TSSpriteButton;

class TSStagePlayLayerGamePrepare :
	public CAStageLayer //, public CCTargetedTouchDelegate
{
protected:
	Define_Sprite_Getter(TSSpriteCommon, label_title);
	Define_Sprite_Getter(TSSpriteCommon, info_dist_back);
	Define_Sprite_Getter(TSSpriteCommon, label_dist_last);
	Define_Sprite_Getter(TSSpriteCommon, label_dist_max);
	Define_Sprite_Getter(TSSpriteButton, button_play);
	//Define_Sprite_Getter(TSSpriteButton, button_shop);
	//Define_Sprite_Getter(TSSpriteButton, button_music);
	Define_Sprite_Getter(TSSpriteButton, button_sound);
	Define_Sprite_Getter(TSSpriteCommon, prepare_player);
	Define_Sprite_Getter(TSSpriteCommon, prepare_shark);
	Define_Sprite_Getter(TSSpriteCommon, prepare_dolphin);
	Define_Sprite_Getter(TSSpriteCommon, prepare_bubble_shark);
	Define_Sprite_Getter(TSSpriteCommon, prepare_bubble_dolphin);

	void _NullGetters()
	{
		Init_Sprite_Getter_Var(label_title);
		Init_Sprite_Getter_Var(info_dist_back);
		Init_Sprite_Getter_Var(label_dist_last);
		Init_Sprite_Getter_Var(label_dist_max);
		//Init_Sprite_Getter_Var(panel_scores);
		Init_Sprite_Getter_Var(button_play);
		//Init_Sprite_Getter_Var(button_shop);
		//Init_Sprite_Getter_Var(button_music);
		Init_Sprite_Getter_Var(button_sound);
		Init_Sprite_Getter_Var(prepare_player);
		Init_Sprite_Getter_Var(prepare_shark);
		Init_Sprite_Getter_Var(prepare_dolphin);
		Init_Sprite_Getter_Var(prepare_bubble_shark);
		Init_Sprite_Getter_Var(prepare_bubble_dolphin);
	}
	void _InitGetters()
	{
		Init_Sprite_Getter(TSSpriteCommon, label_title);
		Init_Sprite_Getter(TSSpriteCommon, info_dist_back);
		Init_Sprite_Getter(TSSpriteCommon, label_dist_last);
		Init_Sprite_Getter(TSSpriteCommon, label_dist_max);
		Init_Sprite_Getter(TSSpriteButton, button_play);
		//Init_Sprite_Getter(TSSpriteButton, button_shop);
		//Init_Sprite_Getter(TSSpriteButton, button_music);
		Init_Sprite_Getter(TSSpriteButton, button_sound);
		Init_Sprite_Getter(TSSpriteCommon, prepare_player);
		Init_Sprite_Getter(TSSpriteCommon, prepare_shark);
		Init_Sprite_Getter(TSSpriteCommon, prepare_dolphin);
		Init_Sprite_Getter(TSSpriteCommon, prepare_bubble_shark);
		Init_Sprite_Getter(TSSpriteCommon, prepare_bubble_dolphin);
	}

	void _findNumberSprites(const char* prefix, CASprite** ppsprs, int size);
	TSGroupNumber _dist_last;
	TSGroupNumber _dist_max;

	bool _bScoreDirty;
	int _score_last;
	int _score_max;
	//void _updateNumber(const char* prefix, int nValue);

	/*
	DELCARE_STATE_HANDLER(Idle);
	DELCARE_STATE_HANDLER(Fadein);
	DELCARE_STATE_HANDLER(Running);
	DELCARE_STATE_HANDLER(OnShop);
	DELCARE_STATE_HANDLER(OnPlay);
	DELCARE_STATE_HANDLER(Fadeout);
	*/
public:
	TSStagePlayLayerGamePrepare(CAStage* pstage, CAStageLayer* playerParent);
	virtual ~TSStagePlayLayerGamePrepare(void);

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

#endif //_STAGE_PLAY_LAYER_GAME_PREPARE_H_
