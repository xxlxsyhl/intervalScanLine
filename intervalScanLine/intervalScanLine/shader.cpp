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


//简单的点光源照射下的绘制
void Shader::run(Model& model)
{
	//对每个面增加点光源和环境光源
	for (int i = 0; i < model.faces.size(); ++i)
	{
		Face& face = model.faces[i];

		for (int j = 0; j < face.vertexIdx.size(); ++j)
		{
			Vertex face_vertex = model.vertexes[face.vertexIdx[j]];
			Vec3f ray_direction = normalize(lightPosition - face_vertex.point);//光线入射反方向
			Vec3f normal = face.normalIdx[j] >= 0 ?
				model.normals[face.normalIdx[j]] : face.normal;
			//求光线入射反方向与面法线/顶点法线的夹角cos
			float cosine = dot(ray_direction, normal);
			//点光源散射颜色
			if (cosine>0.0)face.color += kd*cosine*lightColor;
			//增加环境颜色
			face.color += ambientColor;
		}
		//多边形小块的颜色取顶点的平均颜色
		face.color /= face.vertexIdx.size();

		//控制颜色取值范围在0.0～1.0之间
		if (face.color.r > 1.0f)face.color.r = 1.0f;
		if (face.color.r < 0.0f)face.color.r = 0.0f;
		if (face.color.g > 1.0f)face.color.g = 1.0f;
		if (face.color.g < 0.0f)face.color.g = 0.0f;
		if (face.color.b > 1.0f)face.color.b = 1.0f;
		if (face.color.b < 0.0f)face.color.b = 0.0f;
	}
}






