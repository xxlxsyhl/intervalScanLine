#ifndef MODEL_H
#define MODEL_H

#include "vec.h"
#include <vector>

class Vertex
{
public:
	Point3f point;
	Color3f color;
	//���㷨����
	Vec3f normal;
};

class Face
{
public:
	std::vector<int> vertexIdx;
	//obj�ļ��ṩ���淨���������㷨������������
	std::vector<int> normalIdx;
	//�淨����
	Vec3f normal;
	//Ϊ��Ԫ�ϸ������ƽ����ɫ
	Color3f color;
};

//���ڼ���obj�ļ���ģ��
class Model
{
public:
	//��������
	std::vector<Vertex> vertexes;
	//������
	std::vector<Face> faces;
	//���㷨����
	std::vector<Vec3f> normals;
	//ģ���е�
	Point3f centerPoint;

	//ģ�ͼ���
	bool load(const std::string& path);

	Model() {};
	Model(const std::string& path);
	//����ģ����ת
	void rotate(float* rotateMat, std::vector<Vertex>& vertexes);
};


#endif
