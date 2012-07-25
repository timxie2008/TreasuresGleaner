#if !defined(_STAGE_PLAY_LAYER_COMMON_H_)
#define _STAGE_PLAY_LAYER_COMMON_H_

#include "AStageLayer.h"

class TSStagePlayLayerCommon :
	public CAStageLayer
{
public:
	TSStagePlayLayerCommon(CAStage* pstage, CAStageLayer* playerParent);
	virtual ~TSStagePlayLayerCommon(void);
};

#endif //_STAGE_PLAY_LAYER_COMMON_H_