#ifndef UTILS_H
#define UTILS_H

#include "def.h"
#include <cmath>

static bool isEqualf(float a, float b) 
{ 
	return fabs(a - b) < EPS ? true : false;
}

#endif
