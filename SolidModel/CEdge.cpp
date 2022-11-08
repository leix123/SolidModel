#include "CEdge.h"

CEdge::CEdge()
{
	static int id = 0;
	edgeId = id++;
	halfEdge1 = nullptr;
	halfEdge2 = nullptr;
	preve = nullptr;
	nexte = nullptr;
}

CEdge::~CEdge()
{

}