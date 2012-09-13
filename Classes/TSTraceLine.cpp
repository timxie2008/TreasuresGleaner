
#include "TSTraceLine.h"

TSTraceLine::TSTraceLine()
{
	_nSegments = 0;
	_nYSegmentLast = 2;
	_nYSegmentMax = 5;
	_nYSegmentRange = 3;

	_fOffsetSegBegin = -1;
	_fOffsetSegStone = -1;
	_fOffsetSegEnd = -1;
}

TSTraceLine::~TSTraceLine(void)
{
}

#if 1
static unsigned int _z1 = 12345, _z2 = 12345, _z3 = 12345, _z4 = 12345;
unsigned int _rand()
{
   unsigned int b;
   b  = ((_z1 << 6) ^ _z1) >> 13;
   _z1 = ((_z1 & 4294967294U) << 18) ^ b;
   b  = ((_z2 << 2) ^ _z2) >> 27; 
   _z2 = ((_z2 & 4294967288U) << 2) ^ b;
   b  = ((_z3 << 13) ^ _z3) >> 21;
   _z3 = ((_z3 & 4294967280U) << 7) ^ b;
   b  = ((_z4 << 3) ^ _z4) >> 12;
   _z4 = ((_z4 & 4294967168U) << 13) ^ b;
   unsigned int r = (_z1 ^ _z2 ^ _z3 ^ _z4);
   r &= 0x7fffffff;
   return r;
}

void _seed(unsigned int s)
{
	_z1 = _z2 = _z3 = _z4 = s;
}
#else
static unsigned int _lfsr = 1;
//unsigned period = 0;
unsigned int _rand()
{
	do 
	{
		/* taps: 32 31 29 1; characteristic polynomial: x^32 + x^31 + x^29 + x + 1 */
		_lfsr = (_lfsr >> 1) ^ (-((int)(_lfsr & 1u)) & 0xD0000001u); 
		//++period;
	} 
	while(_lfsr != 1u);

	return _lfsr;
}

void _seed(unsigned int s)
{
	_lfsr = s;
	_lfsr = _rand();
}
#endif

float _frand(float min_, float max_)
{
	float r01 = (float)_rand() / 0x7fffffff;
	return min_ + r01 * (max_ - min_);
}

float TSTraceLine::_frandy(float pmin, float pmax)
{
	pmin = _bottom + (_top - _bottom) * pmin;
	pmax = _bottom + (_top - _bottom) * pmax;
	return _frand(pmin, pmax);
}

//	|-----------|-----------|-----------|-----------|-----------|
//
float TSTraceLine::_frandy()
{
	int n = _nYSegmentLast;
	
	if (n == 0) 
	{
		n = 1;
	}
	else if (n == _nYSegmentMax - 1) 
	{
		n = _nYSegmentMax - 2;
	}
	else
	{
		n = _rand() % _nYSegmentRange - (_nYSegmentRange >> 1);
	
		//n = ..., -1, 0, 1, ...
		n = _nYSegmentLast + n;
		if (n < 0) n = 0;
		if (n >= _nYSegmentMax) n = _nYSegmentMax - 1;
	}
	_nYSegmentLast = n;

	//middle of segment
	float c = n + 0.5f;
	float b, e;
	
	b = c - 0.7f;
	e = c + 0.7f;

	b /= _nYSegmentMax;
	e /= _nYSegmentMax;

	if (b < 0) b = 0;
	if (e > 1.0f) e = 1.0f;

	return _frandy(b, e);
}

float TSTraceLine::_frandx(float offset, float range)
{
	return _frand(offset, offset + range);
}

void TSTraceLine::_createSegment()
{
	if (0 == _nSegments)
	{
		//    ____
		//___/	  \___
		_fOffsetNodeBegin = _left;
		_fOffsetNodeEnd = _left;
		_fOffsetPoint = _left;

		_ptTraceLine[0].x = _frandx(_fOffsetNodeEnd, 0.01f);			_fOffsetNodeEnd += _node_density;
		_ptTraceLine[0].y = _frandy();
		_ptTraceLine[1].x = _frandx(_fOffsetNodeEnd, _node_rand_range);	_fOffsetNodeEnd += _node_density;
		_ptTraceLine[1].y = _frandy();
		_ptTraceLine[2].x = _frandx(_fOffsetNodeEnd, _node_rand_range);	_fOffsetNodeEnd += _node_density;
		_ptTraceLine[2].y = _frandy();
		_ptTraceLine[3].x = _frandx(_fOffsetNodeEnd, _node_rand_range);	_fOffsetNodeEnd += _node_density;
		_ptTraceLine[3].y = _frandy();
	}
	else
	{
		_fOffsetNodeBegin = _fOffsetNodeEnd;

		_ptTraceLine[0] = _ptTraceLine[3];		_fOffsetNodeEnd += _node_density;

		float x1 = _ptTraceLine[3].x + (_ptTraceLine[3].x - _ptTraceLine[2].x);	
		float y1 = _ptTraceLine[3].y + (_ptTraceLine[3].y - _ptTraceLine[2].y);
		if (y1 >= _bottom && y1 <= _top)
		{
		}
		else
		{
			float k = (_ptTraceLine[3].y - _ptTraceLine[2].y) / (_ptTraceLine[3].x - _ptTraceLine[2].x);
			//y = kx + b
			float b = _ptTraceLine[3].y - k * _ptTraceLine[3].x;
			if (y1 <= _bottom)
			{
				y1 = _bottom;
			}
			else if (y1 > _top)
			{
				y1 = _top;
			}
			x1 = (y1 - b) / k;
		}
		_ptTraceLine[1].x = x1;											_fOffsetNodeEnd += _node_density;
		_ptTraceLine[1].y = y1;
		
		_ptTraceLine[2].x = _frandx(_fOffsetNodeEnd, _node_rand_range);	_fOffsetNodeEnd += _node_density;
		_ptTraceLine[2].y = _frandy();

		//float k = (_ptTraceLine[3].y - _ptTraceLine[2].y) / (_ptTraceLine[3].x - _ptTraceLine[2].x);

		_ptTraceLine[3].x = _frandx(_fOffsetNodeEnd, _node_rand_range);	_fOffsetNodeEnd += _node_density;
		_ptTraceLine[3].y = _frandy();
	}

	int i;
	for (i = 0; i < 4; i++)
	{
		_Assert(_ptTraceLine[i].y >= _bottom);
		_Assert(_ptTraceLine[i].y <= _top);
	}
	_nSegments++;
}

void TSTraceLine::init(int seed, float left, float top, float bottom, 
	float node_density, float node_rand_range, float point_density, 
	int nYSegmentMax, int nYSegmentRange)
{
	//_seed = seed;
	_seed(seed);

	_Assert(left > 0 && _left < 1.0f);
	_left = left;

	_Assert(top > bottom);
	_top = top;
	_bottom = bottom;

	_node_density = node_density;
	_node_rand_range = node_rand_range;
	_point_density = point_density;

	_Assert(nYSegmentRange <= nYSegmentMax);

	_nYSegmentMax = nYSegmentMax;
	_nYSegmentLast = _nYSegmentMax / 2; //middle of screen
	_nYSegmentRange = nYSegmentRange;

	_ptLast = CCPointZero;

	_nSegments = 0;
	_createSegment();
}

const CCPoint& TSTraceLine::getLastTracePoint() const
{
	return _ptLast;
}

void TSTraceLine::setSegPoints(float line, float linedelta, float gap, float gapdelta)
{
	_seg_line[0] = line;
	_seg_line[1] = gap;
	_seg_delta[0] = linedelta;
	_seg_delta[1] = gapdelta;

	_fOffsetSegBegin = -1;
	_fOffsetSegStone = -1;
	_fOffsetSegEnd = -1;
}

int TSTraceLine::getNextTracePoint(CCPoint& pos)
{
	float t;
	
	t = (_fOffsetPoint - _fOffsetNodeBegin) / (_fOffsetNodeEnd - _fOffsetNodeBegin);
	if (t > 1.0f)
	{
		_createSegment();
		t = (_fOffsetPoint - _fOffsetNodeBegin) / (_fOffsetNodeEnd - _fOffsetNodeBegin);
		_Assert(t >= 0.0f && t <= 1.0f);
	}
	CCPoint a = bezierat(t, _ptTraceLine[0], _ptTraceLine[1], _ptTraceLine[2], _ptTraceLine[3]);

	_fOffsetPoint += _point_density;

	_ptLast = a;

	if (_fOffsetPoint > _fOffsetSegEnd)
	{
		_fOffsetSegBegin = _fOffsetPoint;
		_fOffsetSegStone = _fOffsetSegBegin + _seg_line[0] + CAUtils::Rand() * _seg_delta[0];
		_fOffsetSegEnd = _fOffsetSegStone + _seg_line[1] + CAUtils::Rand() * _seg_delta[1];
	}
	_Assert(_fOffsetPoint >= _fOffsetSegBegin);
	_Assert(_fOffsetPoint < _fOffsetSegEnd);
	bool online = (_fOffsetPoint < _fOffsetSegStone);

	pos = a;

	return online ? 0 : 1;
}
