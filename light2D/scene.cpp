#include <random>
#include "scene.h"

void Scene::reflect(float ix, float iy, float nx, float ny, float & rx, float & ry) const {
	float idotn2 = (ix * nx + iy * ny) * 2.0f;
	rx = ix - idotn2 * nx;
	ry = iy - idotn2 * ny;
}

bool Scene::refract(float ix, float iy, float nx, float ny, float eta, float & rx, float & ry) const {
	float idotn = ix * nx + iy * ny;
	float k = 1.0f - eta * eta * (1.0f - idotn * idotn);
	if (k < 0.0f)
		return false; // Total internal reflection
	float a = eta * idotn + sqrtf(k);
	rx = eta * ix - a * nx;
	ry = eta * iy - a * ny;
	return true;
}

float Scene::fresnel(float cosi, float cost, float etai, float etat) const {
	float rs = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
	float rp = (etai * cosi - etat * cost) / (etai * cosi + etat * cost);
	return (rs * rs + rp * rp) * 0.5f;
}

float Scene::schlick(float cosi, float cost, float etai, float etat) const {
	float r0 = (etai - etat) / (etai + etat);
	r0 *= r0;
	float a = 1.0f - (etai < etat ? cosi : cost);
	float aa = a * a;
	return r0 + (1.0f - r0) * aa * aa * a;
}

void Scene::gradient(float x, float y, float & nx, float & ny) const {
	nx = (sdf(x + Epslion, y) - sdf(x - Epslion, y)) * (0.5f / Epslion);
	ny = (sdf(x, y + Epslion) - sdf(x, y - Epslion)) * (0.5f / Epslion);
}

Result Scene::intersect(float x, float y) const {
	Result r;
	r.sd = FLT_MAX;
	for each (shared_ptr<Shape> shape in shapes) {
		Result shapeR = shape->intersect(x, y);
		if (shapeR.sd < r.sd) r = shapeR;
	}
	return r;
}

float Scene::sdf(float x, float y) const {
	float sd = FLT_MAX;
	for each (shared_ptr<Shape> shape in shapes) {
		float shapeSD = shape->sdf(x, y);
		if (shapeSD < sd) sd = shapeSD;
	}
	return sd;
}

Color Scene::trace(float ox, float oy, float dx, float dy, int depth) const {
	float t = 1e-3f;
	float sign = intersect(ox, oy).sd > 0.0f ? 1.0f : -1.0f;
	for (int i = 0; i < maxStep && t < maxDistance; i++) {
		float x = ox + dx * t, y = oy + dy * t;
		Result r = intersect(x, y);
		t += r.sd * sign;

		if (r.sd * sign >= Epslion) continue;

		Color & sum = r.emissive;
		Color & refl = r.reflective;

		if (depth > 0 && (!refl.isBlack() || !r.eta.isBlack())) {
			float nx, ny, rx, ry;

			gradient(x, y, nx, ny);
			nx *= sign; ny *= sign;

			if (!r.eta.isBlack()) {
				Color & eta = r.eta;
				Color refr = Colors::White - refl;

				if (eta.r > 0) {
					if (refract(dx, dy, nx, ny, sign < 0.0f ? eta.r : 1.0f / eta.r, rx, ry)) {
						float cosi = -(dx * nx + dy * ny);
						float cost = -(rx * nx + ry * ny);
						sum.r += refr.r * trace(x - nx * Bias, y - ny * Bias, rx, ry, depth - 1).r;
						refl.r = sign < 0.0f ? fresnel(cosi, cost, eta.r, 1.0f) : fresnel(cosi, cost, 1.0f, eta.r);
					} else
						refl.r = 1.0f;
				}
				
				if (eta.g > 0) {
					if (refract(dx, dy, nx, ny, sign < 0.0f ? eta.g : 1.0f / eta.g, rx, ry)) {
						float cosi = -(dx * nx + dy * ny);
						float cost = -(rx * nx + ry * ny);
						sum.g += refr.g * trace(x - nx * Bias, y - ny * Bias, rx, ry, depth - 1).g;
						refl.g = sign < 0.0f ? fresnel(cosi, cost, eta.g, 1.0f) : fresnel(cosi, cost, 1.0f, eta.g);
					} else
						refl.g = 1.0f;
				}

				if (eta.b > 0) {
					if (refract(dx, dy, nx, ny, sign < 0.0f ? eta.b : 1.0f / eta.b, rx, ry)) {
						float cosi = -(dx * nx + dy * ny);
						float cost = -(rx * nx + ry * ny);
						sum.b += refr.b * trace(x - nx * Bias, y - ny * Bias, rx, ry, depth - 1).b;
						refl.b = sign < 0.0f ? fresnel(cosi, cost, eta.b, 1.0f) : fresnel(cosi, cost, 1.0f, eta.b);
					} else
						refl.b = 1.0f;
				}
			}

			if (!refl.isBlack()) {
				reflect(dx, dy, nx, ny, rx, ry);
				sum += refl * trace(x + nx * Bias, y + ny * Bias, rx, ry, depth - 1);
			}

		}
		return sum;
	}
	return Colors::Black;
}

Color Scene::sample(float x, float y) const {
	const static float PI = 3.14159265358f;
	const static float TWO_PI = 2 * PI;
	static std::mt19937 random;
	Color sum;
	for (int i = 0; i < N; i++) {
		// float theta = Maths::TWO_PI * RAND;
		// float theta = Maths::TWO_PI * i / N;
		float theta = TWO_PI * (i + (float)random() / random.max()) / N; // Jittered-Sampling
		sum += trace(x, y, cos(theta), sin(theta), maxDepth);
	}
	return sum / (float)N;
}
