
#include "AWorld.h"
#include "AStage.h"
#include "AStageLayer.h"
#include "TSSpriteMissle.h"
#include "TSSpriteCommon.h"

TSSpriteMissle::TSSpriteMissle(CAStageLayer* player, const CCPoint& pos, const string& state) : CASprite(player, "missle")
{
	this->setPos(pos);
	this->setState(state);

	_rotLast = 0;

	_psprWarning = new TSSpriteCommon(player, "missle_warning");
	CCSize size = CAWorld::getScreenSize();
	_psprWarning->setPos(ccp(1.0f * size.width, pos.y));
	_psprWarning->setState("stand");
	_psprWarning->setFollowCamera(false);
	player->addSprite(_psprWarning);

	_psprTarget = null;
}

TSSpriteMissle::~TSSpriteMissle(void)
{
	if (null != _psprTarget)
	{
		_psprTarget->release();
		_psprTarget = null;
	}
}

void TSSpriteMissle::onStateChanged(const string& olds, const string& news)
{
	this->switchPose(news);
	if (news == "dismiss")
	{
		if (_psprWarning && _psprWarning->getState() != "dismiss")
		{
			_psprWarning->setState("dismiss");
		}
		//create some boooom effects
	}
}

void TSSpriteMissle::onUpdate()
{
	CASprite::onUpdate();

	float angle = 0;
	if (null != _psprWarning)
	{
		CCPoint pos;

		pos = _psprWarning->getPos();
		pos.y = this->getPos().y;
		_psprWarning->setPos(pos);

		//missle vbb
		CCRect rect = getViewBoundingBox();
		CCRect rectLive;
		rectLive.origin = ccp(0, 0);
		rectLive.size = CCSize(1, 1);
		CAWorld::percent2view(rectLive.size);
		if (CCRect::CCRectIntersectsRect(rect, rectLive))
		{
			_psprWarning->killMyself();
			_psprWarning = null;

		}
		//if missle visible, missle will NOT track player any more
		if (null != _psprTarget && null != _psprWarning)
		{
			CCPoint posT = _psprTarget->getPos();
			CCPoint posM = this->getPos();
			CCPoint pos = posT - posM;
			angle = CAUtils::getRotation(pos.y, -pos.x) * 1.0f;
		}
	}
	
	if (!_animRot.isValid())
	{
		float rotNew = _rotLast;
		if (_rotLast > angle) rotNew = -16 + angle;
		else rotNew = 16 + angle;
		//rotNew = angle;
		_animRot.init(_pLayer->getTimeNow(), 0.12f, _rotLast, rotNew);
		_rotLast = rotNew;
	}
	else
	{
		this->setMoveDirection(_animRot.getValue(_pLayer->getTimeNow()));
	}
}

