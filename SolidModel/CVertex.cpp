#include "CVertex.h"

CVertex::CVertex()
{
	static int id = 0;
	vertexId = id++;

	VCoord[0] = VCoord[1] = VCoord[2] = 0.0;
	nextv = prevv = nullptr;
	vhe = nullptr;
}

CVertex::~CVertex()
{
}

void CVertex::SetCoord(double* point)
{
	VCoord[0] = point[0];
	VCoord[1] = point[1];
	VCoord[2] = point[2];
}