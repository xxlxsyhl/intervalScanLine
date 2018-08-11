#ifndef INTERVALSCANLINE_H
#define INTERVALSCANLINE_H

#include "model.h"
#include "vec.h"
#include "def.h"
#include "shader.h"
#include <vector>

struct Edge;
struct PolygonPG;

//即作为边表也作为活化边表
struct Edge
{
	int id;		//边所属多边形的编号
	float x;	//边的上端点的x坐标
	float dx;	//相邻两条扫描线交点的x坐标差,-1/k
	int dy;		//边跨越的扫描线数目-----作为活化边时用
	int ymax;	//边的上端的y坐标
};

//即作为多边形表也作为活化多边形表
struct Polygon
{
	int id;				//多边形编号
	float a, b, c, d;	//多边形所在平面的方程系数
	int dy;				//多边形跨跃的扫描线数目---作为活化多边形时用
	int ymax;			//多边形的上端的
	bool flag;			//标记IN类型还是OUT类型，取：POLYGON_IN、POLYGON_OUT
};

//区间扫描线
class IntervalScanLine 
{
	//用于显示
	friend class Display;

public:
	//帧缓冲
	float *frameBuffer;

	IntervalScanLine(int width, int height);
	~IntervalScanLine();

	void setSize(int width, int height);
	void getSize(int& width, int& height);

	//执行算法
	void run(Model& model, Shader& shader);

private:
	//窗口宽高和背景色
	int width, height;
	Color3f  bgColor;

	//多边形表和边表
	std::vector<std::vector<Polygon>> polygonTable;
	std::vector<std::vector<Edge>> edgeTable;

	//活化多边形表和活化边表
	std::vector<Polygon> activePolygonTable;
	std::vector<Edge> activeEdgeTable;

	//内存释放
	void release();
	//构建多边形表、边表
	void buildTable(Model& model);
	//找离视点最近的多边形
	Polygon closestPolygon(const float x, const float y);
	//获取多边形颜色
	Color3f getPolygonColor(const Model &model, const int id);
	//更新多边形表
	void updateActivePolygonTable(const Edge &edge);
	//更新边表
	void updateActiveEdgeTable();
};

#endif
