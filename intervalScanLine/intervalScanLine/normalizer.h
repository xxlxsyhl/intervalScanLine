#ifndef NORMALIZER_H
#define NORMALIZER_H

#include "model.h"

class Normalizer {
public:
	Normalizer(int _width, int _height) :width(_width), height(_height) {};
	void run(Model &model);

private:
	int width, height;
};


#endif
