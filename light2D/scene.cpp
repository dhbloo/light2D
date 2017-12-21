#include "scene.h"
#include "shapes.h"
#include "math.h"

Result Scene::intersectOp(Result a, Result b) {
	Result r = a.sd > b.sd ? b : a;
	r.sd = a.sd > b.sd ? a.sd : b.sd;
	return r;
}

Result Scene::subtractOp(Result a, Result b) {
	Result r = a;
	r.sd = (a.sd > -b.sd) ? a.sd : -b.sd;
	return r;
}

Result Scene::scene(float x, float y) {
	Result a{ ShapeSDF::circle(x, y, 0.5f, 0.5f, 0.1f), 2.0f * Colors::White };
	Result b{ ShapeSDF::circle(x, y, 0.77f, 0.4f, 0.07f), 1.7f * Colors::Green };
	Result d{ ShapeSDF::box(x, y, 0.4f, 0.8f, -0.3f, 0.1f, 0.06f), Colors::Black, Colors::White * 0.2f, Color(1.f, 0.6f, 0.9f) };
	return unionOp(unionOp(a, b), roundOp(d, 0.03f));
}

void Scene::gradient(float x, float y, float & nx, float & ny) {
	nx = (scene(x + Epslion, y).sd - scene(x - Epslion, y).sd) * (0.5f / Epslion);
	ny = (scene(x, y + Epslion).sd - scene(x, y - Epslion).sd) * (0.5f / Epslion);
}

void Scene::reflect(float ix, float iy, float nx, float ny, float & rx, float & ry) {
	float idotn2 = (ix * nx + iy * ny) * 2.0f;
	rx = ix - idotn2 * nx;
	ry = iy - idotn2 * ny;
}

bool Scene::refract(float ix, float iy, float nx, float ny, float eta, float & rx, float & ry) {
	float idotn = ix * nx + iy * ny;
	float k = 1.0f - eta * eta * (1.0f - idotn * idotn);
	if (k < 0.0f)
		return false; // Total internal reflection
	float a = eta * idotn + sqrtf(k);
	rx = eta * ix - a * nx;
	ry = eta * iy - a * ny;
	return true;
}

Color Scene::trace(float ox, float oy, float dx, float dy, int depth) {
	float t = 1e-3f;
	float sign = scene(ox, oy).sd > 0.0f ? 1.0f : -1.0f;
	for (int i = 0; i < MaxStep && t < MaxDistance; i++) {
		float x = ox + dx * t, y = oy + dy * t;
		Result r = scene(x, y);
		if (r.sd * sign < Epslion) {
			Color & sum = r.emissive;
			Color & refl = r.reflectivity;
			if (depth > 0 && (!refl.isBlack() || !r.eta.isBlack())) {
				float nx, ny, rx, ry;

				gradient(x, y, nx, ny);
				nx *= sign; ny *= sign;
				if (!r.eta.isBlack()) {
					if (refract(dx, dy, nx, ny, sign < 0.0f ? r.eta.r : 1.0f / r.eta.r, rx, ry))
						sum.r += ((Colors::White - refl) * trace(x - nx * Bias, y - ny * Bias, rx, ry, depth - 1)).r;
					else
						refl.r = Colors::White.r; // Total internal reflection

					if (refract(dx, dy, nx, ny, sign < 0.0f ? r.eta.g : 1.0f / r.eta.g, rx, ry))
						sum.g += ((Colors::White - refl) * trace(x - nx * Bias, y - ny * Bias, rx, ry, depth - 1)).g;
					else
						refl.g = Colors::White.g;

					if (refract(dx, dy, nx, ny, sign < 0.0f ? r.eta.b : 1.0f / r.eta.b, rx, ry))
						sum.b += ((Colors::White - refl) * trace(x - nx * Bias, y - ny * Bias, rx, ry, depth - 1)).b;
					else
						refl.b = Colors::White.b;
				}
				if (!refl.isBlack()) {
					reflect(dx, dy, nx, ny, rx, ry);
					sum += refl * trace(x + nx * Bias, y + ny * Bias, rx, ry, depth - 1);
				}

			}
			return sum;
		}
		t += r.sd;
	}
	return Colors::Black;
}

Color Scene::sample(float x, float y) {
	Color sum;
	for (int i = 0; i < N; i++) {
		// float theta = Maths::TWO_PI * RAND;
		// float theta = Maths::TWO_PI * i / N;
		float theta = Maths::TWO_PI * (i + RAND) / N; // Jittered-Sampling
		sum += trace(x, y, cos(theta), sin(theta), MaxDepth);
	}
	return sum / (float)N;
}
