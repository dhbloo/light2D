#pragma once

#include "shape.h"

class ShapeBox : public Shape {
private:
	float sx, sy;
	float theta;

public:
	ShapeBox(float x, float y, float sx, float sy, float theta = 0.0f) : Shape(x, y), sx(sx), sy(sy), theta(theta) {}
	virtual ~ShapeBox() {}

	virtual float sdf(float x, float y) const {
		float costheta = cosf(theta), sintheta = sinf(theta);
		float dx = fabs((x - this->x) * costheta + (y - this->y) * sintheta) - sx;
		float dy = fabs((y - this->y) * costheta - (x - this->x) * sintheta) - sy;
		float ax = fmaxf(dx, 0.0f), ay = fmaxf(dy, 0.0f);
		return fminf(fmaxf(dx, dy), 0.0f) + sqrtf(ax * ax + ay * ay);
	}
};