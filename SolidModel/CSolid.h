#pragma once
class CFace;
class CEdge;
class CVertex;
class CSolid
{
public:
	int solidId;		//ʵ����
	CSolid* prevs;		//ǰһ��ʵ��
	CSolid* nexts;		//��һ��ʵ��
	CFace* sface;		//ָ���һ����
	CEdge* sedge;		//ָ�����
	CVertex* svertex;	//ָ�����

public:
	CSolid();
	virtual ~CSolid();
	CVertex* findVertex(double* point);
};

