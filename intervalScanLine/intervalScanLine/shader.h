#ifndef SHADER_H
#define SHADER_H

#include "Vec.h"
#include "Model.h"

const float kd = 0.8;//散射反照率

class Shader
{
public:
	Shader();
	//设置光照位置
	void setLightPosition(const Point3f &lightPosition);
	//设置点光源颜色
	void setLightColor(const Color3f &lightColor);
	//设置环境光
	void setAmbientColor(const Color3f &ambientColor);
	//着色
	void run(Model& model);

private:
	//点光源位置
	Point3f lightPosition;
	//点光源颜色
	Color3f lightColor;
	//环境光
	Color3f ambientColor;
};

#endif