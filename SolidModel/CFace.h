#pragma once
class CLoop;
class CSolid;
class CFace
{
public:
	int faceId;
	CFace* prevf;			//ǰһ����
	CFace* nextf;			//��һ����
	CLoop* floop;			//ָ����
	CSolid* fsolid;			//������

public:
	CFace();
	virtual ~CFace();

};

