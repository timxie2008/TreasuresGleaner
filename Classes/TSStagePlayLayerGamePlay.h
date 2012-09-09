#if !defined(_STAGE_PLAY_LAYER_GAME_PLAY_H_)
#define _STAGE_PLAY_LAYER_GAME_PLAY_H_

#include "AStageLayer.h"

class TSSpritePlayer;
class TSSpriteCommon;
class TSSpriteButton;
#include "TSGroupNumber.h"

class TSStagePlayLayerGamePlay :
	public CAStageLayer //, public CCTargetedTouchDelegate
{
protected:
	Define_Sprite_Getter(TSSpritePlayer, player);
	Define_Sprite_Getter(TSSpriteCommon, rider);
	Define_Sprite_Getter(TSSpriteButton, button_pause);

	void _NullGetters()
	{
		Init_Sprite_Getter_Var(player);
		Init_Sprite_Getter_Var(rider);
		Init_Sprite_Getter_Var(button_pause);
	}

	void _InitGetters()
	{
		Init_Sprite_Getter(TSSpritePlayer, player);
		//Init_Sprite_Getter(TSSpriteCommon, rider);
		Init_Sprite_Getter(TSSpriteButton, button_pause);
	}

	void _findNumberSprites(const char* prefix, CASprite** ppsprs, int size);
	TSGroupNumber _distance;

	string _state;
	
	float _fDistanceInPixel;
	float _fPlayerSpeedInPixel;
	float _fPlayerSpeedAcc;
	float _fPlayerSpeedMax;

	float _fPlayerSpeedLast;
	CAAnimated2Floats _animPlayerSpeed;

	int _getDistance();
	
	CASprite* _psprIndicators[18];

	//_TSTraceLine _traceline;

	void _updateStageOffset();
	void _checkFishes();
	void _checkRewards();

	//void _updateNumber(const char* prefix, int nValue);

	/*
	DELCARE_STATE_HANDLER(Idle);
	DELCARE_STATE_HANDLER(Fadein);
	DELCARE_STATE_HANDLER(Running);
	DELCARE_STATE_HANDLER(OnResume);
	DELCARE_STATE_HANDLER(OnRestart);
	DELCARE_STATE_HANDLER(Fadeout);
	*/
public:
	TSStagePlayLayerGamePlay(CAStage* pstage, CAStageLayer* playerParent);
	virtual ~TSStagePlayLayerGamePlay(void);

	virtual string debuglog();

	virtual void onEnter();
	virtual void onUpdate();
	virtual void onExit();

	virtual void pause(bool bsub = false);
	virtual void resume(bool bsub = false);

	virtual void onEvent(CAEvent* pevt);

	virtual bool checkCondition(CAState* from, const CATransition& trans);

	virtual void onStateBegin(CAState* from, void* param);
	//virtual void onStateUpdate(CAState* from, size_t counter, ccTime time);
	virtual void onStateEnd(CAState* from, void* param);
};

#endif //_STAGE_PLAY_LAYER_GAME_PLAY_H_
