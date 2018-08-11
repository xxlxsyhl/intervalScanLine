#ifndef SHADER_H
#define SHADER_H

#include "Vec.h"
#include "Model.h"

const float kd = 0.8;//ɢ�䷴����

class Shader
{
public:
	Shader();
	//���ù���λ��
	void setLightPosition(const Point3f &lightPosition);
	//���õ��Դ��ɫ
	void setLightColor(const Color3f &lightColor);
	//���û�����
	void setAmbientColor(const Color3f &ambientColor);
	//��ɫ
	void run(Model& model);

private:
	//���Դλ��
	Point3f lightPosition;
	//���Դ��ɫ
	Color3f lightColor;
	//������
	Color3f ambientColor;
};

#endif