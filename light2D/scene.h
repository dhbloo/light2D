#pragma once

#include <list>
#include <memory>
#include "shape.h"

using std::list;
using std::shared_ptr;

class Scene {
private:
	const float Epslion = 1e-6f;
	const float Bias = 1e-4f;
	int maxDepth = 3;
	int maxStep = 64;
	float maxDistance = 5.0f;
	int N = 16;

	list<shared_ptr<Shape>> shapes;

	void reflect(float ix, float iy, float nx, float ny, float & rx, float & ry) const;
	bool refract(float ix, float iy, float nx, float ny, float eta, float & rx, float & ry) const;
	float fresnel(float cosi, float cost, float etai, float etat) const;
	float schlick(float cosi, float cost, float etai, float etat) const;
	Color beerLambert(Color a, float d) const;
	void gradient(float x, float y, float & nx, float & ny) const;

public:
	Scene() {}
	~Scene() {}

	void clear() { shapes.clear(); }

	Scene & operator+= (shared_ptr<Shape> shape) { shapes.push_back(shape); return *this; }

	Result intersect(float x, float y) const;
	float sdf(float x, float y) const;

	Color trace(float ox, float oy, float dx, float dy, int depth) const;
	Color sample(float x, float y) const;
};