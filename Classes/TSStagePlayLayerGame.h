#if !defined(_STAGE_PLAY_LAYER_GAME_H_)
#define _STAGE_PLAY_LAYER_GAME_H_

#include "AStageLayer.h"

#if defined(WIN32)
#define _DEBUG_INFO
#endif

class TSSpritePlayer;
class TSSpriteCommon;
class TSSpriteButton;

class TSStagePlayLayerGame :
	public CAStageLayer //, public CCTargetedTouchDelegate
{
protected:
#if defined(_DEBUG_INFO)
	CCLabelTTF*		_pInfo;
#endif
	void _updateNumber(const char* prefix, int nValue);

public:
	TSStagePlayLayerGame(CAStage* pstage, CAStageLayer* playerParent);
	virtual ~TSStagePlayLayerGame(void);

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

#endif //_STAGE_PLAY_LAYER_GAME_H_
