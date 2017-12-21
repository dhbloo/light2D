#pragma once

namespace ShapeSDF {
	float circle(float x, float y, float cx, float cy, float r);
	float plane(float x, float y, float px, float py, float nx, float ny);
	float segment(float x, float y, float ax, float ay, float bx, float by);
	float capsule(float x, float y, float ax, float ay, float bx, float by, float r);
	float box(float x, float y, float cx, float cy, float theta, float sx, float sy);
	float triangle(float x, float y, float ax, float ay, float bx, float by, float cx, float cy);
}