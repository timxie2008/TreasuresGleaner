#if !defined(_TSSPRITE_MISSLE_H_)
#define _TSSPRITE_MISSLE_H_

#include "ASprite.h"

class TSSpriteCommon;

class TSSpriteMissle :
	public CASprite
{
protected:
	float _rotLast;
	TSSpriteCommon* _psprWarning;
	CAAnimated2Floats _animRot;
	
	float _fTailPos;

	CASprite* _psprTarget;
	TSSpriteCommon* _createCommonSprite(const char* name, const char* state, const CCPoint& pos, bool autoremove = false, float scale = 1.0f);
public:
	TSSpriteMissle(CAStageLayer* palyer, const CCPoint& pos, const string& state);
	virtual ~TSSpriteMissle(void);

	void setTarget(CASprite* target) { _psprTarget = target; target->retain(); }
	void onStateChanged(const string& olds, const string& news);

	virtual void onUpdate();
};

#endif //_TSSPRITE_MISSLE_H_