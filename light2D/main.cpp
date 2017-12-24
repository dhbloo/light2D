#include <iostream>
#include <windows.h>
#include "window.h"
#include "scene.h"
#include "ShapeCircle.h"
#include "ShapeBox.h"
#include "ShapeTriangle.h"

using namespace std;

/*
void createScene(Scene & scene) {
	scene += make_shared<ShapeCircle>(0.5f, 0.2f, 0.1f)->setEmissive(2.0f * Colors::White);
	scene += make_shared<ShapeCircle>(0.77f, 0.4f, 0.07f)->setEta(2.0f * Colors::White);
	scene += make_shared<ShapeBox>(0.4f, 0.8f, 0.1f, 0.06f, -0.3f)
		->setReflective(Colors::White * .01f)
		->setEta(Color(0.5f, 1.0f, 0.8f) * 3.f);
}
*/

void createScene(Scene & scene) {
	scene += make_shared<ShapeCircle>(-.25f, -.25f, 0.3f)->setEmissive(4.f * Colors::White);
	scene += make_shared<ShapeTriangle>(0.4f, 0.5f, 0.65f, 0.16f, 0.7f, 0.7f)->setEta(Color(1.5f, 2.f, 3.f))
		->setAbsorption(Colors::White * 3);
}

int main() {
	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
	Window window(500, 500, "light2d");
	Scene scene;

	createScene(scene);

#pragma omp parallel for schedule(dynamic)
	for (int y = 0; y < window.height(); y++) {
		for (int x = 0; x < window.width(); x++) {
			int colorInt = scene.sample((float)x / window.width(), (float)y / window.height()).toRGBInt();
			if (!window(x, y)) break;
			else *window(x, y) = colorInt;
		}
		window.update();
	}
	cout << "Render finished." << endl;

	while (window.dispatch());
}