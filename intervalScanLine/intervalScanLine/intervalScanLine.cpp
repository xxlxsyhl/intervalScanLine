#include "intervalScanLine.h"
#include "normalizer.h"
#include "shader.h"
#include "def.h"
#include "utils.h"
#include <algorithm>
#include <iostream>
#include <ctime>
using namespace std;

//������
static bool cmp(const Edge &a, const Edge &b)
{
	int ax = round(a.x), bx = round(b.x);
	//�Ȱ�x���꣬�ٰ�dx�Ĵ�С
	if (ax != bx)	return ax < bx;
	else	return round(a.dx) < round(b.dx);
}

/*
public
*/

//���캯��
IntervalScanLine::IntervalScanLine(int width, int height)
{
	//���ñ���ɫ
	bgColor = Color3f(0.0f, 0.0f, 0.0f);

	//���ô��ڴ�С
	this->width = width;
	this->height = height;

	//����֡����
	frameBuffer = new float[height*width * 3];
	//�����ͷ
	polygonTable.resize(height);
	edgeTable.resize(height);
}

//��������
IntervalScanLine::~IntervalScanLine()
{
	release();
}

//�������ô��ڴ�С
void IntervalScanLine::setSize(int width, int height)
{
	if (width == this->width && height == this->height) return;

	release();

	this->width = width;
	this->height = height;

	//����֡����
	frameBuffer = new float[height*width * 3];
	//�����ͷ
	polygonTable.resize(height);
	edgeTable.resize(height);
}

//��ȡ���ڴ�С
void IntervalScanLine::getSize(int &width, int &height)
{
	width = this->width;
	height = this->height;
}

//����ɨ�����㷨
void IntervalScanLine::run(Model &model, Shader& shader)
{
	//��ʱ
	clock_t t = clock();

	//ģ�Ͳ�������
	Normalizer  normalizer(width, height);
	normalizer.run(model);

	//������ɫ
	shader.run(model);

	//�����߱�ET�Ͷ���α�PT
	buildTable(model);

	activePolygonTable.clear();
	activeEdgeTable.clear();

	//������������ɨ��
	for (int y = height - 1; y > 0; y--)
	{
		//cout << "Y = " << y << endl;

		//������α�������µĶ����
		for (vector<Polygon>::iterator it = polygonTable[y].begin(); it != polygonTable[y].end(); it++)
			activePolygonTable.push_back(*it);

		//���߱�������µı�
		for (vector<Edge>::iterator it = edgeTable[y].begin(); it != edgeTable[y].end(); it++)
			activeEdgeTable.push_back(*it);

		//��x�����dx����
		sort(activeEdgeTable.begin(), activeEdgeTable.end(), cmp);

		//ȡ��һ��߶�
		Edge edge1 = activeEdgeTable[0];
		updateActivePolygonTable(edge1);
		for (int j = 1; j != activeEdgeTable.size(); j++)
		{
			Edge edge2 = activeEdgeTable[j];

			//�Ҿ����ӵ�����Ķ����
			Polygon polygon = closestPolygon((edge1.x + edge2.x) / 2, y);

			//���߶�֮���֡������Ϊ�ö���ε���ɫ
			Color3f color = getPolygonColor(model, polygon.id);

			//���ɨ����Ƭ��
			for (int x = round(edge1.x); x != round(edge2.x); x++)
			{
				frameBuffer[y*width * 3 + x * 3] = color.r;
				frameBuffer[y*width * 3 + x * 3 + 1] = color.g;
				frameBuffer[y*width * 3 + x * 3 + 2] = color.b;
			}

			//���»����α�
			updateActivePolygonTable(edge2);

			//������һ��
			edge1 = edge2;
		}

		//���»�߱���ɾ��ɾ
		updateActiveEdgeTable();

		//�Ƴ��Ѿ������Ļ�����
		int last = 0;
		for (int i = 0; i != activePolygonTable.size(); i++)
		{
			activePolygonTable[i].dy--;
			if (activePolygonTable[i].dy >= 0)
			{
				activePolygonTable[last++] = activePolygonTable[i];
			}
		}
		activePolygonTable.resize(last);
	}

	cout << "����ɨ������ʱ��" << (clock() - t) << "ms" << endl;
}

/*
private
*/

//��ȡ���ɨ���ߵ���ɫ
Color3f IntervalScanLine::getPolygonColor(const Model &model, int id)
{
	//id ����-1��ʾ�Ǳ�������Σ����ر���ɫ��ʣ�µĴ�ģ��ֱ�ӷ���
	if (id == -1)	return bgColor;
	else    return model.faces[id].color;
}

//���»����α�
void IntervalScanLine::updateActivePolygonTable(const Edge &edge)
{

	int id = edge.id;
	for (int i = 0; i != activePolygonTable.size(); i++)
	{
		//����ñ���صĶ����
		if (activePolygonTable[i].id == id)
		{
			//����״̬ȡ��
			activePolygonTable[i].flag = !activePolygonTable[i].flag;
			return;
		}
	}
}

//���»�߱�
void IntervalScanLine::updateActiveEdgeTable()
{
	int last = 0;
	for (int i = 0; i < activeEdgeTable.size(); i++)
	{
		//�޸Ļ�߱��еĲ���
		activeEdgeTable[i].dy--;
		//���������
		activeEdgeTable[i].x += activeEdgeTable[i].dx;
		//���㱣������
		if (activeEdgeTable[i].dy > 0) {
			activeEdgeTable[last++] = activeEdgeTable[i];
		}
	}
	activeEdgeTable.resize(last);
}


//Ѱ�����ӵ�����Ĵ���IN״̬�Ķ����
Polygon IntervalScanLine::closestPolygon(const float x, const float y)
{
	float maxZ = MIN_FlOAT;
	Polygon polygon;
	//�������л�����
	for (int i = 0; i != activePolygonTable.size(); i++)
	{
		//ֻ��IN״̬����αȽ����ֵ
		if (activePolygonTable[i].flag == POLYGON_IN)
		{
			float a = activePolygonTable[i].a, b = activePolygonTable[i].b, c = activePolygonTable[i].c, d = activePolygonTable[i].d;
			//����ƽ�淽�̼��㣨x, y����ߴ������
			float z = (isEqualf(c, 0)) ? 0 : (-d - a*x - b*y) / c;
			if (z >= maxZ)
			{
				maxZ = z;
				polygon = activePolygonTable[i];
			}
		}
	}
	return polygon;
}

//���ɱ߱�Ͷ���α�
void IntervalScanLine::buildTable(Model &model) 
{
	polygonTable.clear();
	edgeTable.clear();

	polygonTable.resize(height);
	edgeTable.resize(height);

	//��ӱ��������������α�
	Polygon bg;
	bg.id = -1, bg.dy = height - 1, bg.flag = POLYGON_OUT, bg.ymax = height - 1;
	bg.a = bg.b = 0, bg.c = 1, bg.d = 0xfffffff;
	polygonTable[bg.ymax].push_back(bg);

	//��ӱ�������εı����߱�����Ϊ���Σ���ֻ���������������
	Edge leftEdge, rightEdge;
	leftEdge.dx = 0, leftEdge.dy = height - 1, leftEdge.id = -1, leftEdge.x = 0, leftEdge.ymax = height - 1;
	rightEdge.dx = 0, rightEdge.dy = height - 1, rightEdge.id = -1, rightEdge.x = width - 1, rightEdge.ymax = height - 1;
	edgeTable[leftEdge.ymax].push_back(leftEdge);
	edgeTable[rightEdge.ymax].push_back(rightEdge);

	//��ÿһ����
	for (int i = 0; i != model.faces.size(); i++) 
	{
		float minY = MAX_FLOAT, maxY = MIN_FlOAT;

		Polygon polygon;
		polygon.id = i;

		const Face& face = model.faces[i];

		/*��������߱�*/
		for (int j = 0; j != face.vertexIdx.size(); j++) 
		{
			Point3f pt1 = model.vertexes[face.vertexIdx[j]].point;
			Point3f pt2 = (j == face.vertexIdx.size() - 1 ? model.vertexes[face.vertexIdx[0]].point : model.vertexes[face.vertexIdx[j + 1]].point);
			if (pt1.y < pt2.y) swap(pt1, pt2);
			//pt1Ϊ�ߵ��϶˵�
			Edge edge;
			edge.dy = round(pt1.y) - round(pt2.y);
			//��ˮƽ��
			if (edge.dy == 0)	continue;

			//������Ϣ
			edge.id = polygon.id;
			edge.x = pt1.x;
			edge.dx = -(pt1.x - pt2.x) / (pt1.y - pt2.y);
			edge.ymax = round(pt1.y);

			//����������߱�
			edgeTable[edge.ymax].push_back(edge);

			//��¼����ε����½�
			minY = min(minY, pt2.y);
			maxY = max(maxY, pt1.y);
		}

		/*�����������α�*/
		polygon.dy = round(maxY) - round(minY);
		if (polygon.dy > 0 && maxY > 0 && minY < height)
		{
			//����ƽ�淽�̲����
			Point3f v = model.vertexes[face.vertexIdx[0]].point;
			polygon.a = face.normal.x;
			polygon.b = face.normal.y;
			polygon.c = face.normal.z;
			polygon.d = -(polygon.a*v.x + polygon.b*v.y + polygon.c*v.z);
			polygon.ymax = round(maxY);
			//ȫ����ʼ��ΪOUT���͵Ķ����
			polygon.flag = POLYGON_OUT;
			//������μ���������α�
			polygonTable[round(maxY)].push_back(polygon);
		}
	}
}


//�ͷ�����ڴ�
void IntervalScanLine::release()
{
	if (frameBuffer != NULL)
	{
		delete[] frameBuffer;
		frameBuffer = NULL;
	}
}
