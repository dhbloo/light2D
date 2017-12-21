#include "shapes.h"
#include "math.h"

float ShapeSDF::circle(float x, float y, float cx, float cy, float r) {
	float ux = x - cx, uy = y - cy;
	return sqrt(ux * ux + uy * uy) - r;
}

float ShapeSDF::plane(float x, float y, float px, float py, float nx, float ny) {
	return (x - px) * nx + (y - py) * ny;
}

float ShapeSDF::segment(float x, float y, float ax, float ay, float bx, float by) {
	float vx = x - ax, vy = y - ay, ux = bx - ax, uy = by - ay;
	float t = fmaxf(fminf((vx * ux + vy * uy) / (ux * ux + uy * uy), 1.0f), 0.0f);
	float dx = vx - ux * t, dy = vy - uy * t;
	return sqrtf(dx * dx + dy * dy);
}

float ShapeSDF::capsule(float x, float y, float ax, float ay, float bx, float by, float r) {
	return segment(x, y, ax, ay, bx, by) - r;
}

float ShapeSDF::box(float x, float y, float cx, float cy, float theta, float sx, float sy) {
	float costheta = cosf(theta), sintheta = sinf(theta);
	float dx = fabs((x - cx) * costheta + (y - cy) * sintheta) - sx;
	float dy = fabs((y - cy) * costheta - (x - cx) * sintheta) - sy;
	float ax = fmaxf(dx, 0.0f), ay = fmaxf(dy, 0.0f);
	return fminf(fmaxf(dx, dy), 0.0f) + sqrtf(ax * ax + ay * ay);
}

float ShapeSDF::triangle(float x, float y, float ax, float ay, float bx, float by, float cx, float cy) {
	float d = fminf(fminf(
		segment(x, y, ax, ay, bx, by),
		segment(x, y, bx, by, cx, cy)),
		segment(x, y, cx, cy, ax, ay));
	return (bx - ax) * (y - ay) > (by - ay) * (x - ax) &&
		(cx - bx) * (y - by) > (cy - by) * (x - bx) &&
		(ax - cx) * (y - cy) > (ay - cy) * (x - cx) ? -d : d;
}
