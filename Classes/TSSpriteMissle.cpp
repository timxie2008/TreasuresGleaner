
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
	_fTailPos = 9999999;

	_psprWarning = new TSSpriteCommon(player, "missle_warning");
	CCSize size = CAWorld::getScreenSize();
	_psprWarning->setPos(ccp(1.0f * size.width, pos.y));
	_psprWarning->setState("stand");
	_psprWarning->setFollowCamera(false);
	player->addSprite(_psprWarning);

	this->layer()->stage()->playEffect("missle_warning");

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
		TSSpriteCommon* pspr = _createCommonSprite("missle_boom", "stand", this->getPos(), true, 1.2f);
		pspr->setVertexZ(this->getVertexZ() + 0.2f);
		_pLayer->addSprite(pspr);
	}
}

TSSpriteCommon* TSSpriteMissle::_createCommonSprite(const char* name, const char* state, const CCPoint& pos, bool autoremove, float scale)
{
	TSSpriteCommon* pspr = new TSSpriteCommon(_pLayer, name);
	pspr->setPos(pos);
	pspr->setScl(scale);
	pspr->setState(state);
	if (autoremove)
	{
		pspr->setDeadPose(state);
	}
	return pspr;
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
			this->layer()->stage()->playEffect("missle_launch");
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
	
	float delta_angle = 7;
	if (!_animRot.isValid())
	{
		float rotNew = _rotLast;
		if (_rotLast > angle) rotNew = -delta_angle + angle;
		else rotNew = delta_angle + angle;
		//rotNew = angle;
		_animRot.init(_pLayer->getTimeNow(), 0.18f, _rotLast, rotNew);
		_rotLast = rotNew;
	}
	else
	{
		this->setMoveDirection(_animRot.getValue(_pLayer->getTimeNow()));
	}

	if (null == _psprWarning)
	{
		CCPoint pos = this->getPos();
		float dist = 0.03f;
		CAWorld::percent2view(dist, true);
		if (pos.x + dist < _fTailPos)
		{
			_fTailPos = pos.x;

			char szPose[32];
			sprintf(szPose, "cloud_01_%02d", (int)(CAUtils::Rand() * 2));
			pos.x += dist;
			TSSpriteCommon* pspr = _createCommonSprite("cloud", szPose, pos, true, 0.6f);
			pspr->setVertexZ(this->getVertexZ() + 0.2f);
			_pLayer->addSprite(pspr);
		}
	}
}

