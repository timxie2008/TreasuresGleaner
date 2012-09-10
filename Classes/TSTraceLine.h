#if !defined(_TSTRACE_LINE_H_)
#define _TSTRACE_LINE_H_

#include "AObject.h"

class TSTraceLine
{
protected:
	int _nSegments;
	float _fOffsetNodeBegin;
	float _fOffsetPoint;
	float _fOffsetNodeEnd;
	CCPoint _ptTraceLine[4];
	int _nYSegmentLast;
	int _nYSegmentMax;
	int _nYSegmentRange;
	void _createSegment();

	float _frandy(float pmin, float pmax);
	float _frandy();
	float _frandx(float offset, float range);

    float _left;
    float _top;
    float _bottom;
    float _node_density;
	float _node_rand_range;
    float _point_density;

	CCPoint _ptLast;
public:
	TSTraceLine();
	virtual ~TSTraceLine(void);

	void init(int seed, float left, float top, float bottom, 
		float node_density, float node_rand_range, float point_density, 
		int nYSegmentMax = 5, int nYSegmentRange = 3);

	const CCPoint& getLastTracePoint() const;
	CCPoint getNextTracePoint();
};

#endif //_TSTRACE_LINE_H_
