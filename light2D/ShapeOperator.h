#pragma once

#include "shape.h"

class ShapeOperator : public Shape {
public:
	ShapeOperator() : Shape() {}
};

class OpIntersect : public ShapeOperator {
	shared_ptr<Shape> a, b;
public:
	OpIntersect(shared_ptr<Shape> a, shared_ptr<Shape> b) : a(a), b(b) {}

	virtual float sdf(float x, float y) const {
		return fmaxf(a->sdf(x, y), b->sdf(x, y));
	}
	virtual Result intersect(float x, float y) const {
		Result ra = a->intersect(x, y);
		Result rb = b->intersect(x, y);
		Result r = ra.sd > rb.sd ? rb : ra;
		r.sd = ra.sd > rb.sd ? ra.sd : rb.sd;
		return r;
	}
};

class OpSubtract : public ShapeOperator {
	shared_ptr<Shape> a, b;
public:
	OpSubtract(shared_ptr<Shape> a, shared_ptr<Shape> b) : a(a), b(b) {}

	virtual float sdf(float x, float y) const {
		float sda = a->sdf(x, y);
		float sdb = b->sdf(x, y);
		return sda > -sdb ? sda : -sdb;
	}
	virtual Result intersect(float x, float y) const {
		Result ra = a->intersect(x, y);
		Result rb = b->intersect(x, y);
		if (ra.sd <= -rb.sd)
			ra.sd = -rb.sd;
		return ra;
	}
};

class OpComplement : public ShapeOperator {
	shared_ptr<Shape> a;
public:
	OpComplement(shared_ptr<Shape> a) : a(a) {}

	virtual float sdf(float x, float y) const { return -a->sdf(x, y); }
	virtual Result intersect(float x, float y) const {
		Result r = a->intersect(x, y);
		return r.sd = -r.sd, r;
	}
};

class OpRound : public ShapeOperator {
	shared_ptr<Shape> a;
	float r;
public:
	OpRound(shared_ptr<Shape> a, float r) : a(a), r(r) {}

	virtual float sdf(float x, float y) const { return a->sdf(x, y) - r; }
	virtual Result intersect(float x, float y) const {
		Result ra = a->intersect(x, y);
		return ra.sd -= r, ra;
	}
};