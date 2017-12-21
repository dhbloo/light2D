#include <iostream>
#include "window.h"
#include "scene.h"
#include "color.h"

using namespace std;

int main() {
	Window window(500, 500, "light2d");

#pragma omp parallel for schedule(dynamic)
	for (int y = 0; y < window.height(); y++) {
		for (int x = 0; x < window.width(); x++) {
			int colorInt = Scene::sample((float)x / window.width(), (float)y / window.height()).toRGBInt();
			if (!window(x, y)) break;
			else *window(x, y) = colorInt;
		}
		window.update();
	}
	cout << "Render finished." << endl;

	while (window.dispatch());
}