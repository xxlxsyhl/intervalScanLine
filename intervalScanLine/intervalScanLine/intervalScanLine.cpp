#include "intervalScanLine.h"
#include "normalizer.h"
#include "shader.h"
#include "def.h"
#include "utils.h"
#include <algorithm>
#include <iostream>
#include <ctime>
using namespace std;

//排序函数
static bool cmp(const Edge &a, const Edge &b)
{
	int ax = round(a.x), bx = round(b.x);
	//先按x坐标，再按dx的大小
	if (ax != bx)	return ax < bx;
	else	return round(a.dx) < round(b.dx);
}

/*
public
*/

//构造函数
IntervalScanLine::IntervalScanLine(int width, int height)
{
	//设置背景色
	bgColor = Color3f(0.0f, 0.0f, 0.0f);

	//设置窗口大小
	this->width = width;
	this->height = height;

	//分配帧缓冲
	frameBuffer = new float[height*width * 3];
	//分配表头
	polygonTable.resize(height);
	edgeTable.resize(height);
}

//析构函数
IntervalScanLine::~IntervalScanLine()
{
	release();
}

//重新设置窗口大小
void IntervalScanLine::setSize(int width, int height)
{
	if (width == this->width && height == this->height) return;

	release();

	this->width = width;
	this->height = height;

	//分配帧缓冲
	frameBuffer = new float[height*width * 3];
	//分配表头
	polygonTable.resize(height);
	edgeTable.resize(height);
}

//获取窗口大小
void IntervalScanLine::getSize(int &width, int &height)
{
	width = this->width;
	height = this->height;
}

//区间扫描线算法
void IntervalScanLine::run(Model &model, Shader& shader)
{
	//计时
	clock_t t = clock();

	//模型参数调整
	Normalizer  normalizer(width, height);
	normalizer.run(model);

	//光照着色
	shader.run(model);

	//构建边表ET和多边形表PT
	buildTable(model);

	activePolygonTable.clear();
	activeEdgeTable.clear();

	//从上往下逐行扫描
	for (int y = height - 1; y > 0; y--)
	{
		//cout << "Y = " << y << endl;

		//向活化多边形表中添加新的多边形
		for (vector<Polygon>::iterator it = polygonTable[y].begin(); it != polygonTable[y].end(); it++)
			activePolygonTable.push_back(*it);

		//向活化边表中添加新的边
		for (vector<Edge>::iterator it = edgeTable[y].begin(); it != edgeTable[y].end(); it++)
			activeEdgeTable.push_back(*it);

		//按x坐标和dx排序
		sort(activeEdgeTable.begin(), activeEdgeTable.end(), cmp);

		//取出一活化边对
		Edge edge1 = activeEdgeTable[0];
		updateActivePolygonTable(edge1);
		for (int j = 1; j != activeEdgeTable.size(); j++)
		{
			Edge edge2 = activeEdgeTable[j];

			//找距离视点最近的多边形
			Polygon polygon = closestPolygon((edge1.x + edge2.x) / 2, y);

			//将边对之间的帧缓冲设为该多边形的颜色
			Color3f color = getPolygonColor(model, polygon.id);

			//填充扫描线片段
			for (int x = round(edge1.x); x != round(edge2.x); x++)
			{
				frameBuffer[y*width * 3 + x * 3] = color.r;
				frameBuffer[y*width * 3 + x * 3 + 1] = color.g;
				frameBuffer[y*width * 3 + x * 3 + 2] = color.b;
			}

			//更新活化多边形表
			updateActivePolygonTable(edge2);

			//处理下一对
			edge1 = edge2;
		}

		//更新活化边表，该删的删
		updateActiveEdgeTable();

		//移除已经结束的活化多边形
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

	cout << "区间扫描线用时：" << (clock() - t) << "ms" << endl;
}

/*
private
*/

//获取填充扫描线的颜色
Color3f IntervalScanLine::getPolygonColor(const Model &model, int id)
{
	//id 等于-1表示是背景多边形，返回背景色，剩下的从模型直接返回
	if (id == -1)	return bgColor;
	else    return model.faces[id].color;
}

//更新活化多边形表
void IntervalScanLine::updateActivePolygonTable(const Edge &edge)
{

	int id = edge.id;
	for (int i = 0; i != activePolygonTable.size(); i++)
	{
		//找与该边相关的多边形
		if (activePolygonTable[i].id == id)
		{
			//将其状态取反
			activePolygonTable[i].flag = !activePolygonTable[i].flag;
			return;
		}
	}
}

//更新活化边表
void IntervalScanLine::updateActiveEdgeTable()
{
	int last = 0;
	for (int i = 0; i < activeEdgeTable.size(); i++)
	{
		//修改活化边表中的参数
		activeEdgeTable[i].dy--;
		//浮点数相加
		activeEdgeTable[i].x += activeEdgeTable[i].dx;
		//满足保留条件
		if (activeEdgeTable[i].dy > 0) {
			activeEdgeTable[last++] = activeEdgeTable[i];
		}
	}
	activeEdgeTable.resize(last);
}


//寻找离视点最近的处于IN状态的多边形
Polygon IntervalScanLine::closestPolygon(const float x, const float y)
{
	float maxZ = MIN_FlOAT;
	Polygon polygon;
	//遍历所有活化多边形
	for (int i = 0; i != activePolygonTable.size(); i++)
	{
		//只对IN状态多边形比较深度值
		if (activePolygonTable[i].flag == POLYGON_IN)
		{
			float a = activePolygonTable[i].a, b = activePolygonTable[i].b, c = activePolygonTable[i].c, d = activePolygonTable[i].d;
			//根据平面方程计算（x, y）左边处的深度
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

//生成边表和多边形表
void IntervalScanLine::buildTable(Model &model) 
{
	polygonTable.clear();
	edgeTable.clear();

	polygonTable.resize(height);
	edgeTable.resize(height);

	//添加背景多边形至多边形表
	Polygon bg;
	bg.id = -1, bg.dy = height - 1, bg.flag = POLYGON_OUT, bg.ymax = height - 1;
	bg.a = bg.b = 0, bg.c = 1, bg.d = 0xfffffff;
	polygonTable[bg.ymax].push_back(bg);

	//添加背景多边形的边至边表，背景为矩形，但只添加左右两条竖边
	Edge leftEdge, rightEdge;
	leftEdge.dx = 0, leftEdge.dy = height - 1, leftEdge.id = -1, leftEdge.x = 0, leftEdge.ymax = height - 1;
	rightEdge.dx = 0, rightEdge.dy = height - 1, rightEdge.id = -1, rightEdge.x = width - 1, rightEdge.ymax = height - 1;
	edgeTable[leftEdge.ymax].push_back(leftEdge);
	edgeTable[rightEdge.ymax].push_back(rightEdge);

	//对每一个面
	for (int i = 0; i != model.faces.size(); i++) 
	{
		float minY = MAX_FLOAT, maxY = MIN_FlOAT;

		Polygon polygon;
		polygon.id = i;

		const Face& face = model.faces[i];

		/*构建分类边表*/
		for (int j = 0; j != face.vertexIdx.size(); j++) 
		{
			Point3f pt1 = model.vertexes[face.vertexIdx[j]].point;
			Point3f pt2 = (j == face.vertexIdx.size() - 1 ? model.vertexes[face.vertexIdx[0]].point : model.vertexes[face.vertexIdx[j + 1]].point);
			if (pt1.y < pt2.y) swap(pt1, pt2);
			//pt1为边的上端点
			Edge edge;
			edge.dy = round(pt1.y) - round(pt2.y);
			//非水平边
			if (edge.dy == 0)	continue;

			//填充边信息
			edge.id = polygon.id;
			edge.x = pt1.x;
			edge.dx = -(pt1.x - pt2.x) / (pt1.y - pt2.y);
			edge.ymax = round(pt1.y);

			//将变加入分类边表
			edgeTable[edge.ymax].push_back(edge);

			//记录多边形的上下界
			minY = min(minY, pt2.y);
			maxY = max(maxY, pt1.y);
		}

		/*构建分类多边形表*/
		polygon.dy = round(maxY) - round(minY);
		if (polygon.dy > 0 && maxY > 0 && minY < height)
		{
			//计算平面方程并填充
			Point3f v = model.vertexes[face.vertexIdx[0]].point;
			polygon.a = face.normal.x;
			polygon.b = face.normal.y;
			polygon.c = face.normal.z;
			polygon.d = -(polygon.a*v.x + polygon.b*v.y + polygon.c*v.z);
			polygon.ymax = round(maxY);
			//全部初始化为OUT类型的多边形
			polygon.flag = POLYGON_OUT;
			//将多边形加入分类多边形表
			polygonTable[round(maxY)].push_back(polygon);
		}
	}
}


//释放相关内存
void IntervalScanLine::release()
{
	if (frameBuffer != NULL)
	{
		delete[] frameBuffer;
		frameBuffer = NULL;
	}
}
