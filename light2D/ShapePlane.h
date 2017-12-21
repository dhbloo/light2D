#pragma once

#include "shape.h"

class ShapePlane : public Shape {
private:
	float nx, ny;

public:
	ShapePlane(float x, float y, float nx, float ny) : Shape(x, y), nx(nx), ny(ny) {}
	virtual ~ShapePlane() {}

	virtual float sdf(float x, float y) const {
		return (x - this->x) * nx + (y - this->y) * ny;
	}
};