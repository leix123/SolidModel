#pragma once
class CHalfEdge;
class CFace;
class CLoop
{
public:
	int loopId;				//�����
	CLoop* prevl;			//ָ��ǰһ����
	CLoop* nextl;			//ָ���һ����
	CHalfEdge* ledge;		//ָ���׸����
	CFace* lface;			//ָ���Ӧ��

public:
	CLoop();
	virtual ~CLoop();
	//void Set

};

