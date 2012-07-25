#if !defined(_TSSPRITE_BUBBLES_H_)
#define _TSSPRITE_BUBBLES_H_
#include "ASprite.h"

class TSSpriteBubble :
	public CASprite
{
protected:
	virtual void _onAnimationStop();
	void _on_pop(EStateFlag flag);
public:
	TSSpriteBubble(CAStageLayer* palyer, const char* name);
	virtual ~TSSpriteBubble(void);

	void onStateChanged(const string& olds, const string& news);
};

#endif //_TSSPRITE_BUBBLES_H_
