#pragma once

#include "shape.h"

class ShapeCircle : public Shape {
private:
	float r;

public:
	ShapeCircle(float x, float y, float r) : Shape(x, y), r(r) {}
	virtual ~ShapeCircle() {}

	virtual float sdf(float x, float y) const {
		float ux = x - this->x, uy = y - this->y;
		return sqrt(ux * ux + uy * uy) - r;
	}
};