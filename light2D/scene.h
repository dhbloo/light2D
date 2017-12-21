#pragma once

#include "color.h"

struct Result { float sd; Color emissive; Color reflectivity; Color eta; };

namespace Scene {
	static int MaxDepth = 4;
	static int MaxStep = 128;
	static float MaxDistance = 2.0f;
	static float Epslion = 1e-6f;
	static float Bias = 1e-4f;
	static int N = 256;

	inline Result unionOp(Result a, Result b) { return a.sd < b.sd ? a : b; }
	inline Result complementOp(Result a) { return a.sd = -a.sd, a; }
	Result intersectOp(Result a, Result b);
	Result subtractOp(Result a, Result b);
	inline Result roundOp(Result a, float r) { return a.sd -= r, a; }
	
	Result scene(float x, float y);

	void gradient(float x, float y, float & nx, float & ny);
	void reflect(float ix, float iy, float nx, float ny, float & rx, float & ry);
	bool refract(float ix, float iy, float nx, float ny, float eta, float & rx, float & ry);
	Color trace(float ox, float oy, float dx, float dy, int depth);
	Color sample(float x, float y);
}

