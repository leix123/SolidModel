#include "CHalfEdge.h"

CHalfEdge::CHalfEdge()
{
	static int id = 0;
	halfEdgeId = id++;
	prv = nxt = nullptr;
	hev = nullptr;
	wloop = nullptr;
	hee = nullptr;
}

CHalfEdge::~CHalfEdge()
{

}

void CHalfEdge::SetVertex(CVertex* vertex)
{
	hev = vertex;
}