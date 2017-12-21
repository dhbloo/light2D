#pragma once

#include "ShapeSegment.h"

class ShapeCapsule : public ShapeSegment {
private:
	float r;

public:
	ShapeCapsule(float ax, float ay, float bx, float by, float r) : ShapeSegment(ax, ay, bx, by), r(r) {}
	virtual ~ShapeCapsule() {}

	virtual float sdf(float x, float y) const {
		return ShapeSegment::sdf(x, y) - r;
	}
};