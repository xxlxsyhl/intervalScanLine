#include "normalizer.h"
#include <algorithm>
using namespace std;

void Normalizer::run(Model& model)
{
	Point3f minXYZ(1e35, 1e35, 1e35), maxXYZ(-1e35, -1e35, -1e35);
	Point3f centerXYZ(0.0, 0.0, 0.0);
	//��ÿ����������䡰��Χ�С�
	for (int i = 0; i < model.vertexes.size(); i++)
	{
		const Point3f& v = model.vertexes[i].point;
		minXYZ.x = min(minXYZ.x, v.x);
		minXYZ.y = min(minXYZ.y, v.y);
		minXYZ.z = min(minXYZ.z, v.z);
		maxXYZ.x = max(maxXYZ.x, v.x);
		maxXYZ.y = max(maxXYZ.y, v.y);
		maxXYZ.z = max(maxXYZ.z, v.z);
	}
	
	//ģ�͵��е�
	centerXYZ = (minXYZ + maxXYZ) / 2;

	//���ģ�����ű���
	float maxLen = max(maxXYZ.x-minXYZ.x, maxXYZ.y-minXYZ.y);
	float scale = min(width, height) / maxLen;
	scale = 0.8*scale;

	//����ÿ������
	for (int i = 0; i < model.vertexes.size(); i++)
	{
		Point3f& v = model.vertexes[i].point;
		v.x = (v.x - centerXYZ.x)*scale + width / 2;
		v.y = (v.y - centerXYZ.y)*scale + height / 2;
		v.z = (v.z - centerXYZ.z)*scale;

	}
	model.centerPoint = Point3f(width / 2, height / 2, 0);
}