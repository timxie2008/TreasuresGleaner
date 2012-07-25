
#include "TSStagePlayLayerCommon.h"

TSStagePlayLayerCommon::TSStagePlayLayerCommon(CAStage* pstage, CAStageLayer* playerParent) : CAStageLayer(pstage, playerParent)
{
	_Trace("%s allocated", __FUNCTION__);
}

TSStagePlayLayerCommon::~TSStagePlayLayerCommon(void)
{
	_Trace("%s destroyed", __FUNCTION__);
}
