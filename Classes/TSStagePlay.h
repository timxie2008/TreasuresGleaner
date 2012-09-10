#if !defined(_TSSTAGE_PLAY_H_)
#define _TSSTAGE_PLAY_H_

#include "AStage.h"

class TSStagePlay :
	public CAStage
{
public:
	TSStagePlay(const char* name);
	virtual ~TSStagePlay(void);

	virtual void onEnter();
};

#endif //_TSSTAGE_PLAY_H_