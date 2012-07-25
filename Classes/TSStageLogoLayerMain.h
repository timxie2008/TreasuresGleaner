#if !defined(_STAGE_LOGO_LAYER_LOGO_H_)
#define _STAGE_LOGO_LAYER_LOGO_H_

#include "AStageLayer.h"

class TSStageLogoLayerMain:
	public CAStageLayer
{
protected:
	string _state;
	ccTime _timeState;
	void _setState(const string& s);
	ccTime _getStateTime();
	CAStage* _pstageNext;
	CCLabelTTF* _pInfo;
public:
	TSStageLogoLayerMain(CAStage* pstage, CAStageLayer* playerParent);
	virtual ~TSStageLogoLayerMain(void);

	virtual void onEnter();
	virtual void onUpdate();
	virtual void onExit();
};

#endif //_STAGE_LOGO_LAYER_LOGO_H_