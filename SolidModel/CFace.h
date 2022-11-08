#pragma once
class CLoop;
class CSolid;
class CFace
{
public:
	int faceId;
	CFace* prevf;			//前一个面
	CFace* nextf;			//后一个面
	CLoop* floop;			//指向环列
	CSolid* fsolid;			//所在体

public:
	CFace();
	virtual ~CFace();

};

