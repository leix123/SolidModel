#include "CSolid.h"
#include "CVertex.h"

CSolid::CSolid() 
{
	static int id = 0;
	solidId = id++;
	prevs = nexts = nullptr;
	sface = nullptr;
	sedge = nullptr;
	svertex = nullptr;
}

CSolid::~CSolid()
{
	
}

CVertex* CSolid::findVertex(double* point)
{
	CVertex* vertex = svertex;
	while (vertex != nullptr) {
		if (vertex->VCoord[0] == point[0] && vertex->VCoord[1] == point[1] && vertex->VCoord[2] == point[2]) {
			return vertex;
		}
		else {
			vertex = vertex->nextv;
		}
	}
	return vertex;
}