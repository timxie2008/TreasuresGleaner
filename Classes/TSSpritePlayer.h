#if !defined(_TSSPRITE_PALYER_H_)
#define _TSSPRITE_PALYER_H_

#include "ASprite.h"

#define EVENT_DIVE_FINISHED	"player.dive.finished"

class TSSpritePlayer :
	public CASprite
{
protected:
	CCRect _rectYard;
	float _direction;

	//CAEventTouch* _pEventTouch;
	float _toRotation;

	virtual void _onAnimationStart();
	virtual void _onAnimationStop();

	void _createBubbles(int c, bool bRight);
	float _timeTouchEvent;
	void _updateDirection(float dir);
	virtual void _adjustPosition(CCPoint& pos);

	void _on_prepare(EStateFlag flag);
	void _on_dive(EStateFlag flag);
	void _on_swiming(EStateFlag flag);
	void _on_waitriding(EStateFlag flag);
	void _on_riding(EStateFlag flag);
	void _on_dead(EStateFlag flag);
	void _on_fadeout(EStateFlag flag);
public:
	TSSpritePlayer(CAStageLayer* palyer, const char* name);
	virtual ~TSSpritePlayer(void);

    virtual void release(void);

	string debuglog() const;

#	define PS_Prepare		"prepare"
#	define PS_Dive			"dive"
#	define PS_Swiming		"swiming"
#	define PS_WaitRiding	"waitriding"
#	define PS_Riding		"riding"
#	define PS_Dead			"dead"
#	define PS_Fadeout		"fadeout"

	void onStateChanged(const string& olds, const string& news);

	virtual void onUpdate();
	//virtual void setMoveSpeed(float s);

	bool onEvent(CAEvent* pEvent);
};

#endif //_TSSPRITE_PALYER_H_