#if !defined(_TSSPRITE_PALYER_H_)
#define _TSSPRITE_PALYER_H_

#include "ASprite.h"

#define EVENT_DIVE_FINISHED	"player.dive.finished"

class TSSpriteCommon;
class TSSpritePlayer :
	public CASprite
{
protected:
	CCRect _rectYard;
	float _direction;
	float _vSpeedWhenTouched;

	//float _calcVSpeed(int index);

	//CAEventTouch* _pEventTouch;
	//float _toRotation;

	float _fPlayerHSpeedLast;
	CAAnimated2Floats _animPlayerSpeed;

	float _fPlayerSpeedInPixel;
	float _fPlayerSpeedAcc;
	float _fPlayerSpeedMax;

	TSSpriteCommon* _psprRider;

	virtual void _onAnimationStart();
	virtual void _onAnimationStop();

	void _createBubbles(int c, bool bRight);
	float _timeTouchEvent;

#define INDEX_PLAYER	0
#define INDEX_DOLPHIN	1
#define INDEX_WHALE		2

	typedef struct tagPlayerSpeedParams
	{
		float touch_power;
		float a;
		float v0;
		float v1;
		float hs_up;
		float hs_down;
		float app_frames;
	}
	_TPlayerSpeedParams;
	_TPlayerSpeedParams _speed_params[3];
	//float _swim_a[3], _swim_v0[3], _swim_hs_up[3], _swim_hs_down[3], _swim_app_frames[3];

	//a = acc
	//v0 = v0
	//hs_up ; hspeed for dir > 0, this is a weight for rot
	//frames : the speed of approaching to spec rot
	float _updateVSpeedAndRotation(int index);

	inline float _updateSwim()		{ return _updateVSpeedAndRotation(INDEX_PLAYER);	};
	inline float _updateWhale()		{ return _updateVSpeedAndRotation(INDEX_WHALE);		};
	inline float _updateDolphin()	{ return _updateVSpeedAndRotation(INDEX_DOLPHIN);	};

	float _speed_weight_dolphin, _speed_weight_whale;
	CCPoint _posOffsetDolphin;
	CCPoint _posOffsetWhale;

	virtual void _adjustPosition(CCPoint& pos);
	virtual void _adjustGamePosition(float& x, float& y);

	void _on_prepare(EStateFlag flag);
	void _on_dive(EStateFlag flag);
	void _on_swiming(EStateFlag flag);
	void _on_hurt(EStateFlag flag);
	void _on_riding_dolphin(EStateFlag flag);
	void _on_riding_whale(EStateFlag flag);
	void _on_dead(EStateFlag flag);
	void _on_fadeout(EStateFlag flag);
public:
	TSSpritePlayer(CAStageLayer* palyer, const char* name);
	virtual ~TSSpritePlayer(void);

    virtual void release(void);

	virtual bool isCollidWith(CASprite* pspr, bool bView = true);
	void ride(const string& rider);
	void setSpeedInfo(float fPlayerSpeedInPixel, float fPlayerSpeedAcc, float fPlayerSpeedMax);
	void setDistance4CalculatingHSpeed(float distance);

	string debuglog() const;

#	define PS_Prepare		"prepare"
#	define PS_Dive			"dive"
#	define PS_Swiming		"swiming"
#	define PS_Hurt			"hurt"
#	define PS_RidingDolphin	"riding_dolphin"
#	define PS_RidingWhale   "riding_whale"
#	define PS_Dead			"dead"
#	define PS_Fadeout		"fadeout"

	bool isRidding(const char* state = null) const
	{
		if (null == state)
		{
			return PS_RidingDolphin == getState() || PS_RidingWhale == getState();
		}
		return state == getState();
	}

	void onStateChanged(const string& olds, const string& news);

	virtual void onUpdate();
	//virtual void setMoveSpeed(float s);

	bool onEvent(CAEvent* pEvent);
};

#endif //_TSSPRITE_PALYER_H_