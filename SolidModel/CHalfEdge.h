#pragma once
class CVertex;
class CLoop;
class CEdge;
class CHalfEdge
{
public:
	int halfEdgeId;		//半边编号
	CHalfEdge* prv;		//指向前一条半边
	CHalfEdge* nxt;		//指向后一条半边
	CVertex* hev;		//指向起点
	CLoop* wloop;		//指向对应环
	CEdge* hee;			//指向对应边

public:
	CHalfEdge();
	virtual ~CHalfEdge();
	void SetVertex(CVertex* vertex);

};

