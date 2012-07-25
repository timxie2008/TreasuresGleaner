#if !defined(_TSSPRITE_COMMON_H_)
#define _TSSPRITE_COMMON_H_
#include "ASprite.h"

class TSSpriteCommon :
	public CASprite
{
protected:
	string _overrided_deadpose;
	void _on_state_event(EStateFlag flag);
	virtual void _onAnimationStop();
public:
	TSSpriteCommon(CAStageLayer* palyer, const char* name);
	virtual ~TSSpriteCommon(void);

	void onStateChanged(const string& olds, const string& news);

	virtual void onUpdate();
};

#endif //_TSSPRITE_COMMON_H_
