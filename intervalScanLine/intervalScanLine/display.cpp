#include "display.h"
#include "def.h"
#include "utils.h"

IntervalScanLine * Display::itvScanLine = NULL;
Model * Display::model = NULL;
Shader * Display::shader = NULL;

float Display::rotateMat[9];

//构造函数
Display::Display(std::string winName, Model *model, IntervalScanLine *itvScanLine, Shader *shader)
{
	this->itvScanLine = itvScanLine;
	this->winName = winName;
	this->model = model;
	this->shader = shader;
}


//显示回调函数
void Display::loop()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawPixels(itvScanLine->width, itvScanLine->height, GL_RGB, GL_FLOAT, itvScanLine->frameBuffer);
	glFinish();
}

//freeglut创建窗口并显示
void Display::run(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow(winName.c_str());
	glutDisplayFunc(loop);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboardEvent);
	glutMainLoop();
}

//键盘事件处理
void Display::keyboardEvent(int key, int x, int y)
{
	float theta = 0.0;
	const float PI = 3.1415926;
	float dtheta = PI / 12;
	float n1 = 1.0, n2 = 0.0, n3 = 0.0;
	switch (key)
	{
	case GLUT_KEY_LEFT:
		theta = dtheta;
		n1 = 0.0;
		n2 = 1.0;
		n3 = 0.0;
		break;
	case GLUT_KEY_RIGHT:
		theta = -dtheta;
		n1 = 0.0;
		n2 = 1.0;
		n3 = 0.0;
		break;
	case GLUT_KEY_UP:
		theta = dtheta;
		n1 = 1.0;
		n2 = 0.0;
		n3 = 0.0;
		break;
	case GLUT_KEY_DOWN:
		theta = -dtheta;
		n1 = 1.0;
		n2 = 0.0;
		n3 = 0.0;
		break;
	case GLUT_KEY_PAGE_UP:
		theta = -dtheta;
		n1 = 0.0;
		n2 = 0.0;
		n3 = 1.0;
		break;
	case GLUT_KEY_PAGE_DOWN:
		theta = dtheta;
		n1 = 0.0;
		n2 = 0.0;
		n3 = 1.0;
		break;

	default:
		break;
	}

	float cosine = cos(theta), sine = sin(theta);

	rotateMat[0] = n1*n1 + (1 - n1*n1)*cosine;
	rotateMat[1] = n1*n2*(1 - cosine) + n3*sine;
	rotateMat[2] = n1*n3*(1 - cosine) - n2*sine;

	rotateMat[3] = n1*n2*(1 - cosine) - n3*sine;
	rotateMat[4] = n2*n2*(1 - cosine) + cosine;
	rotateMat[5] = n2*n3*(1 - cosine) + n1*sine;

	rotateMat[6] = n1*n2*(1 - cosine) + n2*sine;
	rotateMat[7] = n2*n3*(1 - cosine) - n1*sine;
	rotateMat[8] = n3*n3 + (1 - n3*n3)*cosine;

	model->rotate(rotateMat, model->vertexes);

	itvScanLine->run(*model, *shader);
	glutPostRedisplay();
}

//处理窗口大小
void Display::reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	itvScanLine->setSize(w, h);
	itvScanLine->run(*model, *shader);
}