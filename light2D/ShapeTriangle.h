#pragma once

#include "shape.h"

class ShapeTriangle : public Shape {
private:
	float bx, by, cx, cy;

	float segment(float x, float y, float ax, float ay, float bx, float by) const {
		float vx = x - ax, vy = y - ay, ux = bx - ax, uy = by - ay;
		float t = fmaxf(fminf((vx * ux + vy * uy) / (ux * ux + uy * uy), 1.0f), 0.0f);
		float dx = vx - ux * t, dy = vy - uy * t;
		return sqrtf(dx * dx + dy * dy);
	}

public:
	ShapeTriangle(float ax, float ay, float bx, float by, float cx, float cy) : Shape(ax, ay), bx(bx), by(by), cx(cx), cy(cy) {}
	virtual ~ShapeTriangle() {}

	virtual float sdf(float x, float y) const {
		float d = fminf(fminf(
			segment(x, y, this->x, this->y, bx, by),
			segment(x, y, bx, by, cx, cy)),
			segment(x, y, cx, cy, this->x, this->y));
		return (bx - this->x) * (y - this->y) > (by - this->y) * (x - this->x) &&
			(cx - bx) * (y - by) > (cy - by) * (x - bx) &&
			(this->x - cx) * (y - cy) > (this->y - cy) * (x - cx) ? -d : d;
	}
};