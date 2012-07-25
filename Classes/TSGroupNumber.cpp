
#include "TSGroupNumber.h"
#include "AWorld.h"
#include "AStageLayer.h"
#include "TSSpriteCommon.h"

TSGroupNumber::TSGroupNumber()
{
	//autorelease();
	_dirty = false;
}

TSGroupNumber::~TSGroupNumber(void)
{
	int i, c = MAX_NUMBER_COUNT;
	for (i = 0; i < c; i++)
	{
		if (null == _sprs[i])
			continue;
		//_sprs[i]->killMyself();
		//_sprs[i]->release();
	}
}

void TSGroupNumber::init(CAStageLayer* player, CASprite** psprNumber, int count, const char* pszCharMap)
{
	_player = player;
	_len = count;
	_charmap = pszCharMap;
	_dirty = true;

	int i;
	for (i = 0; i < MAX_NUMBER_COUNT; i++)
	{
		if (i < count)
		{
			_sprs[i] = psprNumber[i];
			_sprs[i]->setVisible(false);
			//_sprs[i]->retain();
		}
		else
		{
			_sprs[i] = null;
		}
	}
}

void TSGroupNumber::setState(const char* ps)
{
	int i;
	for (i = 0; i < MAX_NUMBER_COUNT; i++)
	{
		if (_sprs[i])
			_sprs[i]->setState(ps);
	}
}

/*
void TSGroupNumber::setLayout(const CCPoint& pos, const CCSize& size)
{
	int i;
	for (i = 0; i < MAX_NUMBER_COUNT; i++)
	{
		_sprs[i]->setPos(pos.x * size.width * i, pos.y + size.height * i);
	}
}
*/

//void TSGroupNumber::setNumber(int number)
void TSGroupNumber::setText(const char* pszText)
{
	if (_text != pszText)
	{
		_text = pszText;
		_dirty = true;
	}
}


static int _goNear(int cur, int to, int range, int step)
{
	if (cur == to) return cur;
	if ((cur - to < 0 && to - cur > (range >> 1)) ||
		(cur - to > 0 && cur - to < (range >> 1)))
	{
		cur += range - step;
	}
	else
	{
		cur += step;
	}
	cur %= range;

	return cur;
}

void TSGroupNumber::onUpdate()
{
	if (!_dirty)
		return;

	_dirty = false;

	//int lenold = _textDisplaying.size();
	int lennew = _text.size();
	int i;
	for (i = 0; i < lennew && i < _len; i++)
	{
		//char chold = i < lenold ? _textDisplaying[i] : 0;
		char chnew = _text[i];
		char szPose[4];
		szPose[0] = chnew;
		szPose[1] = 0;
		unsigned int idx = _charmap.find(szPose);
		if (idx >= 0 && idx < _charmap.size())
		{
			_sprs[i]->setVisible(true);
			_sprs[i]->switchPose(szPose);
		}
	}
	for ( ; i < _len; i++)
	{
		_sprs[i]->setVisible(false);
	}
}

