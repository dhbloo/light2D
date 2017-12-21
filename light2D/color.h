#pragma once

#ifndef _COLOR_H_
#define _COLOR_H_

#include "math.h"

class Color {
public:
	float r, g, b;

	Color() : r(0.f), g(0.f), b(0.f) {}
	Color(float r, float g, float b) : r(r), g(g), b(b) {}
	~Color() {}

	float operator[](UInt8 i) { return (&r)[i]; }

	inline bool operator ==(const Color & color) {
		return color.r == r && color.g == g && color.b == b;
	}

	inline bool isBlack() {
		return r <= Maths::ZERO_EPS && g <= Maths::ZERO_EPS && b <= Maths::ZERO_EPS;
	}

	inline Color operator - () const {
		return Color(-r, -g, -b);
	}

	inline Color operator + (const Color & color) const {
		return Color(r + color.r, g + color.g, b + color.b);
	}

	inline Color operator - (const Color & color) const {
		return Color(r - color.r, g - color.g, b - color.b);
	}

	inline Color operator * (const Color & color) const {
		return Color(r * color.r, g * color.g, b * color.b);
	}

	inline Color operator / (const Color & color) const {
		return Color(r / color.r, g / color.g, b / color.b);
	}

	inline Color operator * (float k) const {
		return Color(r * k, g * k, b * k);
	}

	inline Color operator / (float k) const {
		return Color(r / k, g / k, b / k);
	}

	inline Color & operator+= (const Color & color) {
		r += color.r; g += color.g; b += color.b;
		return *this;
	}

	inline Color & operator-= (const Color & color) {
		r -= color.r; g -= color.g; b -= color.b;
		return *this;
	}

	inline Color & operator*= (const Color & color) {
		r *= color.r; g *= color.g; b *= color.b;
		return *this;
	}

	inline Color & operator/= (const Color & color) {
		r /= color.r; g /= color.g; b /= color.b;
		return *this;
	}

	inline Color & operator*= (float k) {
		r *= k; g *= k; b *= k;
		return *this;
	}

	inline Color & operator/= (float k) {
		r /= k; g /= k; b /= k;
		return *this;
	}

	inline Color & clamp() {
		r = Maths::clamp(r);
		g = Maths::clamp(g);
		b = Maths::clamp(b);
		return *this;
	}

	inline Color & clampNegative() {
		r = r < 0 ? 0 : r;
		g = g < 0 ? 0 : g;
		b = b < 0 ? 0 : b;
		return *this;
	}

	inline Color & gammaCorrect(float gamma) {
		float oneOverGamma = 1 / gamma;
		r = pow(r, oneOverGamma);
		g = pow(g, oneOverGamma);
		b = pow(b, oneOverGamma);
		return *this;
	}

	inline Color & gammaCorrect_inv(float oneOverGamma) {
		r = pow(r, oneOverGamma);
		g = pow(g, oneOverGamma);
		b = pow(b, oneOverGamma);
		return *this;
	}

	inline int toRGBInt() const {
		int ir = int(r * 255 + 0.5);
		int ig = int(g * 255 + 0.5);
		int ib = int(b * 255 + 0.5);
		ir = Maths::clamp(ir, 0, 255);
		ig = Maths::clamp(ig, 0, 255);
		ib = Maths::clamp(ib, 0, 255);
		return (ir << 16) | (ig << 8) | ib;
	}

	friend std::ostream& operator << (std::ostream &s, const Color &c) {
		return s << "color(" << c.r << "," << c.g << "," << c.b << ")";
	}

	inline friend Color operator*(float k, const Color & c) {
		return Color(k * c.r, k * c.g, k * c.b);
	}
};

namespace Colors {
	const Color Black = Color(0.0, 0.0, 0.0);
	const Color White = Color(1.0, 1.0, 1.0);
	const Color Red = Color(1.0, 0.0, 0.0);
	const Color Green = Color(0.0, 1.0, 0.0);
	const Color Blue = Color(0.0, 0.0, 1.0);
	inline Color Random() {
		return Color((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	}
}

#endif