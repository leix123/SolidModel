#pragma once
class CHalfEdge;
class CVertex
{
public:
	int vertexId;		//���
	double VCoord[3];	//��ά����
	CVertex* prevv;		//ǰһ����
	CVertex* nextv;		//��һ����
	CHalfEdge* vhe;		//ָ������

public:
	CVertex();
	virtual ~CVertex();
	void SetCoord(double* point);		//���������
};

