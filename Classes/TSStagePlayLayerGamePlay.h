#if !defined(_STAGE_PLAY_LAYER_GAME_PLAY_H_)
#define _STAGE_PLAY_LAYER_GAME_PLAY_H_

#include "AStageLayer.h"
#include "AMath3d.h"

class TSSpritePlayer;
class TSSpriteCommon;
class TSSpriteButton;

#include "TSGroupNumber.h"
#include "TSTraceLine.h"

class TSStagePlayLayerGamePlay :
	public CAStageLayer //, public CCTargetedTouchDelegate
{
protected:
	Define_Sprite_Getter(TSSpritePlayer, player);
	Define_Sprite_Getter(TSSpriteButton, button_pause);

	void _NullGetters()
	{
		Init_Sprite_Getter_Var(player);
		Init_Sprite_Getter_Var(button_pause);
	}

	void _InitGetters()
	{
		Init_Sprite_Getter(TSSpritePlayer, player);
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
	void _updateScoreBar();

	TSTraceLine _traceline;
	CCPoint	_ptLastBlocker;
	int _traceline_coin2pearl;
	float _traceline_blocker_k;
	float _traceline_blocker_dy_percent_from_center;
	float _traceline_block_density;

	int _nCollected;
	void _addCollected(int c);

	void _updateStageOffset();
	void _checkBlockers();
	void _checkRewards();
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
