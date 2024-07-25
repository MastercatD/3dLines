#include "Segment3D.h"

Segment3D::Segment3D(Vector3D* start, Vector3D* end) {
	this->start = *start;
	this->end = *end;
}

void Segment3D::SetStart(Vector3D start) {
	this->start = start;
}

void Segment3D::SetEnd(Vector3D start) {
	this->end = end;
}

Vector3D* Segment3D::GetStart() {
	return &start;
}

Vector3D* Segment3D::GetEnd() {
	return &end;
}

bool Segment3D::InSegment(double x, double y, double z) {
	double x0, x1, y0, y1, z0, z1;
	if (start.GetX() > end.GetX()) {
		x0 = end.GetX();
		x1 = start.GetX();
	}
	else {
		x0 = start.GetX();
		x1 = end.GetX();
	}
	if (start.GetY() > end.GetY()) {
		y0 = end.GetY();
		y1 = start.GetY();
	}
	else {
		y0 = start.GetY();
		y1 = end.GetY();
	}
	if (start.GetZ() > end.GetZ()) {
		z0 = end.GetZ();
		z1 = start.GetZ();
	}
	else {
		z0 = start.GetZ();
		z1 = end.GetZ();
	}
	return (x >= x0 && x <= x1 && y >= y0 && y <= y1 && z >= z0 && z <= z1);
}