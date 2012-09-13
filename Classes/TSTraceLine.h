#if !defined(_TSTRACE_LINE_H_)
#define _TSTRACE_LINE_H_

#include "AObject.h"

class TSTraceLine
{
protected:
	int _nSegments;
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

	float _seg_line[2];
	float _seg_delta[2];
	float _fOffsetSegBegin;
	float _fOffsetSegStone;
	float _fOffsetSegEnd;

	float _fOffsetNodeBegin;
	float _fOffsetPoint;
	float _fOffsetNodeEnd;
	
	CCPoint _ptLast;
public:
	TSTraceLine();
	virtual ~TSTraceLine(void);

	int getSegmentsCount() const { return _nSegments; }
	float top() const { return _top; }
	float bottom() const { return _bottom; }

	void init(int seed, float left, float top, float bottom, 
		float node_density, float node_rand_range, float point_density, 
		int nYSegmentMax = 5, int nYSegmentRange = 3);

	void setSegPoints(float line, float linedelta, float gap, float gapdelta);

	const CCPoint& getLastTracePoint() const;
	int getNextTracePoint(CCPoint& pos);
};

#endif //_TSTRACE_LINE_H_
