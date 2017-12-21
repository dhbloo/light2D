#pragma once

#include <cmath>
#include <memory>
#include "color.h"

using std::shared_ptr;

class Result {
public:
	float sd;
	Color emissive;
	Color reflective;
	Color eta;
};

class Shape : public std::enable_shared_from_this<Shape> {
protected:
	float x, y;

	Color emissive;
	Color reflective;
	Color eta;

public:
	Shape(float x = 0.f, float y = 0.f) : x(x), y(y) {}
	virtual ~Shape() {}

	shared_ptr<Shape> setEmissive(const Color & c) { emissive = c; return shared_from_this(); }
	shared_ptr<Shape> setReflective(const Color & c) { reflective = c; return shared_from_this(); }
	shared_ptr<Shape> setEta(const Color & c) { eta = c; return shared_from_this(); }

	virtual float sdf(float x, float y) const = 0;
	virtual Result intersect(float x, float y) const {
		return Result{ sdf(x,y), emissive , reflective , eta };
	}
};