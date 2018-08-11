#ifndef INTERVALSCANLINE_H
#define INTERVALSCANLINE_H

#include "model.h"
#include "vec.h"
#include "def.h"
#include "shader.h"
#include <vector>

struct Edge;
struct PolygonPG;

//����Ϊ�߱�Ҳ��Ϊ��߱�
struct Edge
{
	int id;		//����������εı��
	float x;	//�ߵ��϶˵��x����
	float dx;	//��������ɨ���߽����x�����,-1/k
	int dy;		//�߿�Խ��ɨ������Ŀ-----��Ϊ���ʱ��
	int ymax;	//�ߵ��϶˵�y����
};

//����Ϊ����α�Ҳ��Ϊ�����α�
struct Polygon
{
	int id;				//����α��
	float a, b, c, d;	//���������ƽ��ķ���ϵ��
	int dy;				//����ο�Ծ��ɨ������Ŀ---��Ϊ������ʱ��
	int ymax;			//����ε��϶˵�
	bool flag;			//���IN���ͻ���OUT���ͣ�ȡ��POLYGON_IN��POLYGON_OUT
};

//����ɨ����
class IntervalScanLine 
{
	//������ʾ
	friend class Display;

public:
	//֡����
	float *frameBuffer;

	IntervalScanLine(int width, int height);
	~IntervalScanLine();

	void setSize(int width, int height);
	void getSize(int& width, int& height);

	//ִ���㷨
	void run(Model& model, Shader& shader);

private:
	//���ڿ�ߺͱ���ɫ
	int width, height;
	Color3f  bgColor;

	//����α�ͱ߱�
	std::vector<std::vector<Polygon>> polygonTable;
	std::vector<std::vector<Edge>> edgeTable;

	//�����α�ͻ�߱�
	std::vector<Polygon> activePolygonTable;
	std::vector<Edge> activeEdgeTable;

	//�ڴ��ͷ�
	void release();
	//��������α��߱�
	void buildTable(Model& model);
	//�����ӵ�����Ķ����
	Polygon closestPolygon(const float x, const float y);
	//��ȡ�������ɫ
	Color3f getPolygonColor(const Model &model, const int id);
	//���¶���α�
	void updateActivePolygonTable(const Edge &edge);
	//���±߱�
	void updateActiveEdgeTable();
};

#endif
