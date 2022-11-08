#pragma once
class CHalfEdge;
class CVertex
{
public:
	int vertexId;		//序号
	double VCoord[3];	//三维坐标
	CVertex* prevv;		//前一个点
	CVertex* nextv;		//后一个点
	CHalfEdge* vhe;		//指向半边列

public:
	CVertex();
	virtual ~CVertex();
	void SetCoord(double* point);		//输入点坐标
};

