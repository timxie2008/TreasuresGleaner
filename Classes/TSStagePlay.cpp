
#include "TSStagePlay.h"

TSStagePlay::TSStagePlay(const char* name) : CAStage(name)
{
}

TSStagePlay::~TSStagePlay(void)
{
}

void TSStagePlay::onEnter()
{
	CAStage::onEnter();

	//this->setPosition(ccp(200, 100));
	//this->setScale(0.5f);
}
