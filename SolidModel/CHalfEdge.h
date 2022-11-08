#pragma once
class CVertex;
class CLoop;
class CEdge;
class CHalfEdge
{
public:
	int halfEdgeId;		//��߱��
	CHalfEdge* prv;		//ָ��ǰһ�����
	CHalfEdge* nxt;		//ָ���һ�����
	CVertex* hev;		//ָ�����
	CLoop* wloop;		//ָ���Ӧ��
	CEdge* hee;			//ָ���Ӧ��

public:
	CHalfEdge();
	virtual ~CHalfEdge();
	void SetVertex(CVertex* vertex);

};

