#ifndef MODEL_H
#define MODEL_H

#include "vec.h"
#include <vector>

class Vertex
{
public:
	Point3f point;
	Color3f color;
	//顶点法向量
	Vec3f normal;
};

class Face
{
public:
	std::vector<int> vertexIdx;
	//obj文件提供的面法向量（顶点法向量）的索引
	std::vector<int> normalIdx;
	//面法向量
	Vec3f normal;
	//为面元上各顶点的平均颜色
	Color3f color;
};

//用于加载obj文件的模型
class Model
{
public:
	//顶点数组
	std::vector<Vertex> vertexes;
	//面数组
	std::vector<Face> faces;
	//顶点法向量
	std::vector<Vec3f> normals;
	//模型中点
	Point3f centerPoint;

	//模型加载
	bool load(const std::string& path);

	Model() {};
	Model(const std::string& path);
	//处理模型旋转
	void rotate(float* rotateMat, std::vector<Vertex>& vertexes);
};


#endif
