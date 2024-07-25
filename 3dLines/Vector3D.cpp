#include "Vector3D.h"

Vector3D::Vector3D(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

double Vector3D::GetX() {
	return x;
}

double Vector3D::GetY() {
	return y;
}

double Vector3D::GetZ() {
	return z;
}

void Vector3D::SetXYZ(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}