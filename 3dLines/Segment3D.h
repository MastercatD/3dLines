#pragma once
#include "Vector3D.h"
#include <cmath>

class Segment3D {
 private:
  Vector3D start;
  Vector3D end;

 public:
  void SetStart(Vector3D start);
  void SetEnd(Vector3D end);
  Vector3D GetStart();
  Vector3D GetEnd();
  Segment3D();
  Segment3D(Vector3D start, Vector3D end);
  bool InSegment(double x, double y, double z);
};