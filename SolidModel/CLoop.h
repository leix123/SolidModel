#pragma once
class CHalfEdge;
class CFace;
class CLoop
{
public:
	int loopId;				//环编号
	CLoop* prevl;			//指向前一个环
	CLoop* nextl;			//指向后一个环
	CHalfEdge* ledge;		//指向首个半边
	CFace* lface;			//指向对应面

public:
	CLoop();
	virtual ~CLoop();
	//void Set

};

