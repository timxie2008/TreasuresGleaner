#if !defined(_TSSPRITE_BLOCKER_H_)
#define _TSSPRITE_BLOCKER_H_
#include "ASprite.h"

class TSSpriteCommon;
class TSSpriteBlocker :
	public CASprite
{
protected:
	string _overrided_deadpose;
	void _on_state_event(EStateFlag flag);
	virtual void _onAnimationStop();
	TSSpriteCommon* _createCommonSprite(const char* name, const char* state, const CCPoint& pos, bool autoremove = false, float scale = 1.0f);
public:
	TSSpriteBlocker(CAStageLayer* palyer, const char* name);
	virtual ~TSSpriteBlocker(void);

	void setDeadPose(const string& pose) { _overrided_deadpose = pose; }
	void onStateChanged(const string& olds, const string& news);

	virtual void onUpdate();
};

#endif //_TSSPRITE_BLOCKER_H_
