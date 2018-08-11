#include "model.h"
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <omp.h>

using namespace std;

//加载模型
bool Model::load(const string& path)
{
	ifstream file(path);
	if (!file.is_open()) return false;
	string type;

	//加载顶点坐标
	while (file >> type)
	{
		if (type == "v")
		{
			Vertex vt;
			file >> vt.point.x >> vt.point.y >> vt.point.z;
			vertexes.push_back(vt);
		}
		//加载面-顶点索引/纹理索引/法线索引
		else if (type == "f")
		{
			Face face;
			int vIndex, tIndex, nIndex;
			int faceIndex = faces.size();

			while (true)
			{
				char ch = file.get();
				if (ch == ' ') continue;
				else if (ch == '\n' || ch == EOF) break;
				else file.putback(ch);

				//顶点的索引
				file >> vIndex;

				char splitter = file.get();
				nIndex = 0;

				if (splitter == '/')
				{
					splitter = file.get();
					if (splitter == '/')
					{
						//面法向量的index
						file >> nIndex;
					}
					else
					{
						file.putback(splitter);
						//纹理(texture)索引
						file >> tIndex;
						splitter = file.get();
						if (splitter == '/')
						{
							file >> nIndex;
						}
						else file.putback(splitter);
					}
				}
				else file.putback(splitter);

				face.vertexIdx.push_back(vIndex - 1);
				face.normalIdx.push_back(nIndex - 1);
			}
			//计算面元中3点确定的法向量，在obj文件不提供面法向量时使用
			if (face.vertexIdx.size() > 2)
			{
				Point3f &a = vertexes[face.vertexIdx[0]].point,
					&b = vertexes[face.vertexIdx[1]].point, &c = vertexes[face.vertexIdx[2]].point;

				//求法线，并单位化
				Vec3f& normal = normalize(cross(b - a, c - b));

				face.normal = normal;
				faces.push_back(face);
			}
		}
		//加载obj文件提供的法向量，如果有的话
		else if (type == "vn")
		{
			Vec3f vn;
			file >> vn.x >> vn.y >> vn.z;
			normals.push_back(vn);
		}
	}
	file.close();
	return true;
}

Model::Model(const string& path)
{
	if (load(path))
	{
		string face_type;
		cout << "模型" + path + "加载成功！" << endl;
		cout << "面片数：" << faces.size() <<
			"，顶点数：" << vertexes.size() << endl;
	}
	else
	{
		cout << "无法打开obj文件：" + path << endl;
	}
}


//旋转模型
void Model::rotate(float* rotateMat, vector<Vertex>& vertexes)
{
	//顶点
	//#pragma omp parallel for
	for (int i = 0; i < vertexes.size(); i++)
	{
		Point3f point;
		Point3f tmp_point = vertexes[i].point - centerPoint;
		vertexes[i].point.x = rotateMat[0] * tmp_point.x + rotateMat[1] * tmp_point.y + rotateMat[2] * tmp_point.z;
		vertexes[i].point.y = rotateMat[3] * tmp_point.x + rotateMat[4] * tmp_point.y + rotateMat[5] * tmp_point.z;
		vertexes[i].point.z = rotateMat[6] * tmp_point.x + rotateMat[7] * tmp_point.y + rotateMat[8] * tmp_point.z;
		vertexes[i].point += centerPoint;
	}

	//顶点法向
	//#pragma omp parallel for
	for (int i = 0; i < normals.size(); i++)
	{
		Point3f tmp_point = normals[i];
		normals[i].x = rotateMat[0] * tmp_point.x + rotateMat[1] * tmp_point.y + rotateMat[2] * tmp_point.z;
		normals[i].y = rotateMat[3] * tmp_point.x + rotateMat[4] * tmp_point.y + rotateMat[5] * tmp_point.z;
		normals[i].z = rotateMat[6] * tmp_point.x + rotateMat[7] * tmp_point.y + rotateMat[8] * tmp_point.z;

	}

	//面法向
	
	//#pragma omp parallel for
	for (int i = 0; i < faces.size(); i++)
	{
		Point3f tmp_point = faces[i].normal;
		faces[i].normal.x = rotateMat[0] * tmp_point.x + rotateMat[1] * tmp_point.y + rotateMat[2] * tmp_point.z;
		faces[i].normal.y = rotateMat[3] * tmp_point.x + rotateMat[4] * tmp_point.y + rotateMat[5] * tmp_point.z;
		faces[i].normal.z = rotateMat[6] * tmp_point.x + rotateMat[7] * tmp_point.y + rotateMat[8] * tmp_point.z;
	}
}
