#include "Euler.h"
#include<cstdio>


/// <summary>
/// 构造一个点、一个外环、一个面、一个体,是构造实体第一步
/// </summary>
/// <param name="point">一个点的三维左边数组</param>
/// <returns>一个实体</returns>
CSolid* mvsf(double* point)
{
	CVertex* vertex = new CVertex();	//第一个点
	CLoop* loop = new CLoop();			//第一个环
	CFace* face = new CFace();			//第一个面
	CSolid* solid = new CSolid();		//第一个体

	//初始化
	solid->sface = face;				//体的第一个面
	solid->svertex = vertex;			//体的第一个点
	face->fsolid = solid;				//面所在体
	face->floop = loop;					//面的第一个环
	loop->lface = face;					//环所在面

	vertex->SetCoord(point);			//设置点的坐标

	return solid;
}


/// <summary>
/// 生成一个新点,同时生成一条边,连接新点和给定点
/// </summary>
/// <param name="vertex">给定点</param>
/// <param name="loop">点所在环</param>
/// <param name="newVertex">新点坐标</param>
/// <returns>生成的其中一条半边</returns>
CHalfEdge* mev(double* givenPoint, double* point, CLoop* loop)
{
	CVertex* vertex = loop->lface->fsolid->findVertex(givenPoint);
	CHalfEdge* halfEdge1 = new CHalfEdge();					//新半边1
	CHalfEdge* halfEdge2 = new CHalfEdge();					//新半边2
	CEdge* edge = new CEdge();								//新边
	CVertex* newVertex = new CVertex();
	
	if (vertex == nullptr) {
		puts("The given point can't be found in the Solid.");
		return nullptr;
	}

	//初始化
	halfEdge1->hee = halfEdge2->hee = edge;					//新半边对应的边是新边
	halfEdge1->wloop = halfEdge2->wloop = loop;				//新半边所在环
	halfEdge1->hev = vertex;								//半边1的起点为给定点
	halfEdge2->hev = newVertex;								//半边2的起点为新点
	halfEdge1->nxt = halfEdge2;								//半边1的下一个半边是半边2
	halfEdge2->prv = halfEdge1;								//半边2的前一个半边是半边1
	edge->halfEdge1 = halfEdge1;							//新边的两条半边
	edge->halfEdge2 = halfEdge2;
	newVertex->SetCoord(point);								//设置新点坐标

	//新点加入体的点列
	CVertex* vtemp = vertex;								//从给定点找到点列尾,同时添加新点
	while (vtemp->nextv != nullptr) {
		vtemp = vtemp->nextv;
	}
	vtemp->nextv = newVertex;
	newVertex->prevv = vtemp;

	//新边加入体的边列
	CSolid* solid = loop->lface->fsolid;					//找到环所在体
	CEdge* eg = solid->sedge;
	if (eg == nullptr) {									//如果体没有边
		solid->sedge = edge;
	}
	else {													//如果体有边
		while (eg->nexte != nullptr) {
			eg = eg->nexte;
		}
		eg->nexte = edge;
		edge->preve = eg;
	}
	
	//将半边加入环
	if (loop->ledge == nullptr) {							//如果环中没有半边
		loop->ledge = halfEdge1;							//环中第一条半边设为半边1
		halfEdge2->nxt = halfEdge1;							//半边2的下一个半边是半边1
		halfEdge1->prv = halfEdge2;							//半边1的前一个半边是半边2
	}
	else {													//如果环中存在半边
		CHalfEdge* halfEdge = loop->ledge;					//首半边
		while (halfEdge->nxt->hev != vertex) {				//找到下一条半边的起点是给定点的半边
			//printf("%d ", halfEdge->hev->vertexId);
			halfEdge = halfEdge->nxt;
		}
		//printf("44444444444\n");
		halfEdge2->nxt = halfEdge->nxt;						//半边2的下条边设为找到边的下条
		halfEdge->nxt = halfEdge1;							//找到边的下条边为半边1
		halfEdge1->prv = halfEdge;							//半边1的前一个半边是找到边
		halfEdge2->nxt->prv = halfEdge2;					//找到边的原后条边的前条边为半边2
	}

	return halfEdge1;
}

/// <summary>
/// 以给定两个点为端点生成新边,新环,新面
/// v1->v2的半边是内环
/// </summary>
/// <param name="vertex1">尾点</param>
/// <param name="vertex2">头点</param>
/// <param name="loop"></param>
CLoop* mef(double* point1, double* point2, CLoop* loop)
{
	CVertex* vertex1 = loop->lface->fsolid->findVertex(point1);
	CVertex* vertex2 = loop->lface->fsolid->findVertex(point2);
	CHalfEdge* halfEdge1 = new CHalfEdge();					//新半边
	CHalfEdge* halfEdge2 = new CHalfEdge();
	CEdge* edge = new CEdge();								//新边
	CLoop* newLoop = new CLoop();
	CFace* face = new CFace();

	//半边初始化
	halfEdge1->hee = halfEdge2->hee = edge;					//半边对应边
	halfEdge1->hev = vertex1;								//半边1的起点为v1
	halfEdge2->hev = vertex2;								//半边2的起点为v2
	
	//将半边加入环
	CHalfEdge* he1 = loop->ledge;
	CHalfEdge* he2 = loop->ledge;

	while (he1->hev != vertex1) {							//he1起点为v1的半边
		he1 = he1->nxt;
	}
	while (he2->hev != vertex2) {							//he2起点为v2的半边
		he2 = he2->nxt;
	}

	////////////////需要画图理解,我也不愿意看第二遍,不写注释了
	halfEdge1->prv = he1->prv;
	halfEdge1->nxt = he2;
	halfEdge2->prv = he2->prv;
	halfEdge2->nxt = he1;
	he1->prv->nxt = halfEdge1;
	he1->prv = halfEdge2;
	he2->prv->nxt = halfEdge2;
	he2->prv = halfEdge1;


	//新环初始化
	newLoop->ledge = halfEdge1;
	newLoop->lface = face;

	//新环中的半边指向新环
	while (halfEdge1->wloop != newLoop) {
		halfEdge1->wloop = newLoop;
		halfEdge1 = halfEdge1->nxt;
	}														//循环结束后halfEdge1依旧指向原halfEdge1

	loop->ledge = halfEdge2;								//旧环的半边需要更新 旧环为v2->v1所在环
	halfEdge2->wloop = loop;

	//新环加入新面的环列
	CLoop* lp = face->floop;
	if (lp == nullptr)
	{
		face->floop = newLoop;
	}
	else {
		while (lp->nextl != nullptr) {
			lp = lp->nextl;
		}
		lp->nextl = newLoop;
		newLoop->prevl = lp;
	}
	

	//新面初始化
	face->fsolid = loop->lface->fsolid;

	//新面加入体的面列
	CFace* fc = face->fsolid->sface;
	if (fc == nullptr) {
		face->fsolid->sface = face;
	}
	else {
		while (fc->nextf != nullptr) {
			fc = fc->nextf;
		}
		fc->nextf = face;
		face->prevf = fc;
	}

	//边初始化
	edge->halfEdge1 = halfEdge1;						//设置边对应的半边
	edge->halfEdge2 = halfEdge2;

	//边加入体的边列
	CEdge* eg = loop->lface->fsolid->sedge;				//指向体边列的首边
	if (eg == nullptr) {								//如果没有边
		eg = edge;
	}
	else {												//如果存在边
		while (eg->nexte != nullptr) {					//找到尾边
			eg = eg->nexte;
		}
		eg->nexte = edge;								//添加
		edge->preve = eg;
	}

	return newLoop;
}

/// <summary>
/// 删除环中的两个半边,即一条边,同时生成一个内环
/// </summary>
/// <param name="vertex1">一条半边的起点</param>
/// <param name="vertex2">另一条半边的起点,且为内环点</param>
/// <param name="loop">两条半边所在的环</param>
/// <returns>内环</returns>
CLoop* kemr(double* point1, double* point2, CLoop* loop)
{
	CHalfEdge* half = loop->ledge;
	CVertex* vertex1 = loop->lface->fsolid->findVertex(point1);
	CVertex* vertex2 = loop->lface->fsolid->findVertex(point2);
	CLoop* newLoop = new CLoop();								//新内环
	CSolid* solid = loop->lface->fsolid;						//环所在体
	
	//删除环中两条半边
	CHalfEdge* halfEdge1;										//v1->v2的半边
	CHalfEdge* halfEdge2;										//v2->v1的半边
	CHalfEdge* he = loop->ledge;
	while (he->hev != vertex1 || he->nxt->hev != vertex2) {		//找半边1
		he = he->nxt;
	}
	halfEdge1 = he;
	he = he->nxt;
	while (he->hev != vertex2 || he->nxt->hev != vertex1) {		//找半边2，在内环上
		he->wloop = newLoop;									//内环上的半边指向新环
		he = he->nxt;
	}
	halfEdge2 = he;

	//改变内环上半边的指向和外环上半边的指向
	halfEdge1->prv->nxt = halfEdge2->nxt;						//外环he1的前一条
	halfEdge2->nxt->prv = halfEdge1->prv;						//外环he2的后一条

	if (halfEdge1->nxt != halfEdge2) {							//内环非空 ,内环为空时不需要操作
		halfEdge1->nxt->prv = halfEdge2->prv;					//内环he1的后一条
		halfEdge2->prv->nxt = halfEdge1->nxt;					//内环he2的前一条

		//内环初始化
		newLoop->ledge = halfEdge1->nxt;	
	}

	loop->ledge = halfEdge1->prv;
	
	newLoop->lface = loop->lface;

	//内环加入所在面的环列
	CLoop* lp = loop->lface->floop;
	if (lp == nullptr) {
		lp = newLoop;
	}
	else {
		while (lp->nextl != nullptr) {
			lp = lp->nextl;

		}
		lp->nextl = newLoop;
		newLoop->prevl = lp;
	}

	//删除体的边列的对应边
	CEdge* eg = solid->sedge;									
	while (eg != halfEdge1->hee) {
		eg = eg->nexte;
	}
	if (!eg->preve && eg->nexte) {									//eg为边列首边且有下一条边
		solid->sedge = eg->nexte;
		solid->sedge->preve = nullptr;
	}
	else if(eg->preve && eg->nexte){								//在列中
		eg->preve->nexte = eg->nexte;
		eg->nexte->preve = eg->preve;
	}
	else if (eg->preve && !eg->nexte) {								//在列尾且有前一个
		eg->preve->nexte = nullptr;
	}
	else {															//只有一个
		solid->sedge = nullptr;
	}

	delete eg;
	delete halfEdge1;
	delete halfEdge2;
	
	return newLoop;
}

/// <summary>
/// 删除一个面,并将其定义为另一个面的内环,进而在体中生成一个通孔或将两个物体合并成一个物体
/// </summary>
/// <param name="outLoop">外环</param>
/// <param name="inLoop">内环</param>
void kfmrh(CLoop* outLoop, CLoop* inLoop)
{
	CFace* inFace = inLoop->lface;
	inLoop->lface = outLoop->lface;
	//将内环加入到外环所在面的环列
	CLoop* lp = outLoop->lface->floop;
	if (lp == nullptr) {
		outLoop->lface->floop = inLoop;
	}
	else {
		while (lp->nextl != nullptr) {
			lp = lp->nextl;
		}
		lp->nextl = inLoop;
		inLoop->prevl = lp;
	}

	//删除面
	if (inFace->prevf && inFace->nextf) {						//在面列中间
		inFace->nextf->prevf = inFace->prevf;
		inFace->prevf->nextf = inFace->nextf;
	}
	else if (!inFace->prevf && inFace->nextf) {					//为面列第一个且有后一个
		outLoop->lface->fsolid->sface = inFace->nextf;
		inFace->nextf->prevf = nullptr;
	}
	else if (inFace->prevf && !inFace->nextf) {					//为面列最后一个且有前一个
		inFace->prevf->nextf = nullptr;
	}
	else {														//只有一个面
		outLoop->lface->fsolid->sface = nullptr;
	}

	delete inFace;
}


/// <summary>
/// 扫掠操作
/// </summary>
/// <param name="face">底面</param>
/// <param name="dx"></param>
/// <param name="dy"></param>
/// <param name="dz"></param>
void sweeping(CFace* face, double dx, double dy, double dz)
{
	CLoop* loop = face->floop;												//外环
	while (loop) {
		CHalfEdge* halfEdge = loop->ledge;
		CHalfEdge* he = loop->ledge;
		CHalfEdge* halfEdgePrv = halfEdge->prv;
		CHalfEdge* halfEdgeNxt = halfEdge->nxt;
		CLoop* loopNext = loop->nextl;

		double point1[3], point2[3], first[3], last[3];						//p1是底面坐标 p2是平移后的坐标 first是环第一个点的坐标 last是环上一个点的坐标
		point1[0] = he->hev->VCoord[0];
		point1[1] = he->hev->VCoord[1];
		point1[2] = he->hev->VCoord[2];
		first[0] = last[0] = point2[0] = point1[0] + dx;
		first[1] = last[1] = point2[1] = point1[1] + dy;
		first[2] = last[2] = point2[2] = point1[2] + dz;
		mev(point1, point2, loop);											//p2加入环
		
		while(he != halfEdgePrv) {
			he = halfEdgeNxt;
			halfEdgeNxt = halfEdgeNxt->nxt;
			point1[0] = he->hev->VCoord[0];
			point1[1] = he->hev->VCoord[1];
			point1[2] = he->hev->VCoord[2];
			point2[0] = point1[0] + dx;
			point2[1] = point1[1] + dy;
			point2[2] = point1[2] + dz;
			mev(point1, point2, loop);										//加入环
			mef(point2, last, loop);										//增加面
			last[0] = point2[0];
			last[1] = point2[1];
			last[2] = point2[2];
		}
		mef(first, last, loop);												//收尾
		loop = loopNext;
	}
}