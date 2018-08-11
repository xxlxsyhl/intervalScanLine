#include "Shader.h"

using namespace std;

Shader::Shader()
{
	lightColor = Color3f(0.3f, 0.3f, 0.3f);
	ambientColor = Color3f(0.3f, 0.3f, 0.3f);
	lightPosition = Point3f(400.0f, 600.0f, 500.0f);
}

void Shader::setLightPosition(const Point3f &lightPosition)
{
	this->lightPosition = lightPosition;
}

void Shader::setLightColor(const Color3f &lightColor)
{
	this->lightColor = lightColor;
}

void Shader::setAmbientColor(const Color3f &ambientColor)
{
	this->ambientColor = ambientColor;
}


//�򵥵ĵ��Դ�����µĻ���
void Shader::run(Model& model)
{
	//��ÿ�������ӵ��Դ�ͻ�����Դ
	for (int i = 0; i < model.faces.size(); ++i)
	{
		Face& face = model.faces[i];

		for (int j = 0; j < face.vertexIdx.size(); ++j)
		{
			Vertex face_vertex = model.vertexes[face.vertexIdx[j]];
			Vec3f ray_direction = normalize(lightPosition - face_vertex.point);//�������䷴����
			Vec3f normal = face.normalIdx[j] >= 0 ?
				model.normals[face.normalIdx[j]] : face.normal;
			//��������䷴�������淨��/���㷨�ߵļн�cos
			float cosine = dot(ray_direction, normal);
			//���Դɢ����ɫ
			if (cosine>0.0)face.color += kd*cosine*lightColor;
			//���ӻ�����ɫ
			face.color += ambientColor;
		}
		//�����С�����ɫȡ�����ƽ����ɫ
		face.color /= face.vertexIdx.size();

		//������ɫȡֵ��Χ��0.0��1.0֮��
		if (face.color.r > 1.0f)face.color.r = 1.0f;
		if (face.color.r < 0.0f)face.color.r = 0.0f;
		if (face.color.g > 1.0f)face.color.g = 1.0f;
		if (face.color.g < 0.0f)face.color.g = 0.0f;
		if (face.color.b > 1.0f)face.color.b = 1.0f;
		if (face.color.b < 0.0f)face.color.b = 0.0f;
	}
}






