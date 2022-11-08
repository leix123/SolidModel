#pragma once
class CHalfEdge;
class CEdge
{
public:
	int edgeId;
	CHalfEdge* halfEdge1;
	CHalfEdge* halfEdge2;
	CEdge* preve;
	CEdge* nexte;

public:
	CEdge();
	virtual ~CEdge();
};

