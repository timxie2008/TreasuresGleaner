
#include "TSStageTestLayerTest.h"

TSStageTestLayerTest::TSStageTestLayerTest(CAStage* pstage, CAStageLayer* playerParent) : CAStageLayer(pstage, playerParent)
{
	_Trace("%s allocated", __FUNCTION__);
}

TSStageTestLayerTest::~TSStageTestLayerTest(void)
{
	_Trace("%s destroyed", __FUNCTION__);
}
