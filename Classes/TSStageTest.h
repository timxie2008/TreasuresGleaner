#if !defined(_TSSTAGE_TEST_H_)
#define _TSSTAGE_TEST_H_

#include "AStage.h"

class TSStageTest :
	public CAStage
{
public:
	TSStageTest(const char* name);
	virtual ~TSStageTest(void);
};

#endif //_TSSTAGE_TEST_H_