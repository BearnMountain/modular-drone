#pragma once

#include "src/util/defines.h"

struct Vec3 {
	f32 x, y, z;
};

struct Color {
	f32 r, g, b, a;
};

struct CPoint { // colored point
	Vec3 position;
	Color color;
}; 
