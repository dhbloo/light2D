#pragma once

#include "shape.h"

class ShapeSegment : public Shape {
private:
	float bx, by;

public:
	ShapeSegment(float ax, float ay, float bx, float by) : Shape(ax, ay), bx(bx), by(by) {}
	virtual ~ShapeSegment() {}

	virtual float sdf(float x, float y) const {
		float vx = x - this->x, vy = y - this->y, ux = bx - this->x, uy = by - this->y;
		float t = fmaxf(fminf((vx * ux + vy * uy) / (ux * ux + uy * uy), 1.0f), 0.0f);
		float dx = vx - ux * t, dy = vy - uy * t;
		return sqrtf(dx * dx + dy * dy);
	}
};