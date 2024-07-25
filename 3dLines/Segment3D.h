#pragma once
#include "Vector3D.h"

class Segment3D {
private:
	Vector3D start;
	Vector3D end;
public:
	void SetStart(Vector3D start);
	void SetEnd(Vector3D start);
	Vector3D* GetStart();
	Vector3D* GetEnd();
	Segment3D(Vector3D* start, Vector3D* end);
	bool InSegment(double x, double y, double z);
};