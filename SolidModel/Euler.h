#pragma once
#include "CVertex.h"
#include "CHalfEdge.h"
#include "CEdge.h"
#include "CLoop.h"
#include "CFace.h"
#include "CSolid.h"


CSolid* mvsf(double* point);
//void kvsf(CVertex* vertex, CSolid* solid);
CHalfEdge* mev(double* givenPoint, double* point, CLoop* loop);
//void kev();
CLoop* mef(double* point1, double* point2, CLoop* loop);
//void kef();
CLoop* kemr(double* point1, double* point2, CLoop* loop);
//void mekr();
void kfmrh(CLoop* outLoop, CLoop* inLoop);
//void mfkrh();
//void semv();
//void kekv();

void sweeping(CFace* face, double dx, double dy, double dz);

