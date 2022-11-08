#pragma once
class CFace;
class CEdge;
class CVertex;
class CSolid
{
public:
	int solidId;		//实体编号
	CSolid* prevs;		//前一个实体
	CSolid* nexts;		//后一个实体
	CFace* sface;		//指向第一个面
	CEdge* sedge;		//指向边列
	CVertex* svertex;	//指向点列

public:
	CSolid();
	virtual ~CSolid();
	CVertex* findVertex(double* point);
};

