#if !defined(_STAGE_TEST_LAYER_TEST_H_)
#define _STAGE_TEST_LAYER_TEST_H_

#include "AStageLayer.h"

class TSStageTestLayerTest :
	public CAStageLayer
{
public:
	TSStageTestLayerTest(CAStage* pstage, CAStageLayer* playerParent);
	virtual ~TSStageTestLayerTest(void);
};

#endif //_STAGE_TEST_LAYER_TEST_H_