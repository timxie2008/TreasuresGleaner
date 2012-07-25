#if !defined(_TSSPRITE_FISH_H_)
#define _TSSPRITE_FISH_H_

#include "ASprite.h"

class TSSpriteFish :
	public CASprite
{
protected:
	void _on_swim(EStateFlag flag);
public:
	TSSpriteFish(CAStageLayer* palyer, const char* name);
	virtual ~TSSpriteFish(void);

	void onStateChanged(const string& olds, const string& news);

	virtual void onUpdate();
};

#endif //_TSSPRITE_FISH_H_