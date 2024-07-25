#pragma once

class Vector3D {
private:
	double x, y, z;
public:
	Vector3D(double x = 0, double y = 0, double z = 0);
	double GetX();
	double GetY();
	double GetZ();
	void SetXYZ(double x = 0, double y = 0, double z = 0);
};