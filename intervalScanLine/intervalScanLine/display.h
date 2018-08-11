#ifndef DISPLAY_H
#define DISPLAY_H


#include "model.h"
#include "intervalScanLine.h"
#include "shader.h"
#include <freeglut.h>
#include <iostream>
#include <cmath>
#include <string>

class IntervalScanLine;

class Display
{
public:
	std::string winName;
	static IntervalScanLine *itvScanLine;
	static Shader *shader;
	static Model* model;
	//旋转矩阵
	static float rotateMat[9];

	Display(std::string winName, Model *model, IntervalScanLine *itvScanLine, Shader *shader);

	void run(int argc, char **argv);
	static void loop();

	//事件处理
	static void reshape(int w, int h);
	static void keyboardEvent(int key, int x, int y);

};


#endif
