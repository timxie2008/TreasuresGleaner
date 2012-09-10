#if !defined(_TSTRACE_LINE_H_)
#define _TSTRACE_LINE_H_

#include "AObject.h"

class TSTraceLine
{
protected:
	int _nIndexTraceLine;
	CCPoint _ptTraceLine[64];
	
	int _getNodeCount();
	//index = 0, -1, -2, -3, -4 ....
	//index = 0 is the last node
	//if index < -count, return the first node
	CCPoint _getNode(int index);

	float _xTraceLineOffset;

    int _seed;
    float _traceline_left;
    float _traceline_top;
    float _traceline_bottom;
    float _traceline_node_density;
    float _traceline_point_density;
public:
	TSTraceLine();
	virtual ~TSTraceLine(void);

	void init(int seed, float left, float top, float bottom, float node_density, float point_density);

	void addTraceNode(const CCPoint& pt);
	CCPoint getTracePoint(float offset);
};

#endif //_TSTRACE_LINE_H_
