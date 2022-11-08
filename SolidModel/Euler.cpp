#include "Euler.h"
#include<cstdio>


/// <summary>
/// ����һ���㡢һ���⻷��һ���桢һ����,�ǹ���ʵ���һ��
/// </summary>
/// <param name="point">һ�������ά�������</param>
/// <returns>һ��ʵ��</returns>
CSolid* mvsf(double* point)
{
	CVertex* vertex = new CVertex();	//��һ����
	CLoop* loop = new CLoop();			//��һ����
	CFace* face = new CFace();			//��һ����
	CSolid* solid = new CSolid();		//��һ����

	//��ʼ��
	solid->sface = face;				//��ĵ�һ����
	solid->svertex = vertex;			//��ĵ�һ����
	face->fsolid = solid;				//��������
	face->floop = loop;					//��ĵ�һ����
	loop->lface = face;					//��������

	vertex->SetCoord(point);			//���õ������

	return solid;
}


/// <summary>
/// ����һ���µ�,ͬʱ����һ����,�����µ�͸�����
/// </summary>
/// <param name="vertex">������</param>
/// <param name="loop">�����ڻ�</param>
/// <param name="newVertex">�µ�����</param>
/// <returns>���ɵ�����һ�����</returns>
CHalfEdge* mev(double* givenPoint, double* point, CLoop* loop)
{
	CVertex* vertex = loop->lface->fsolid->findVertex(givenPoint);
	CHalfEdge* halfEdge1 = new CHalfEdge();					//�°��1
	CHalfEdge* halfEdge2 = new CHalfEdge();					//�°��2
	CEdge* edge = new CEdge();								//�±�
	CVertex* newVertex = new CVertex();
	
	if (vertex == nullptr) {
		puts("The given point can't be found in the Solid.");
		return nullptr;
	}

	//��ʼ��
	halfEdge1->hee = halfEdge2->hee = edge;					//�°�߶�Ӧ�ı����±�
	halfEdge1->wloop = halfEdge2->wloop = loop;				//�°�����ڻ�
	halfEdge1->hev = vertex;								//���1�����Ϊ������
	halfEdge2->hev = newVertex;								//���2�����Ϊ�µ�
	halfEdge1->nxt = halfEdge2;								//���1����һ������ǰ��2
	halfEdge2->prv = halfEdge1;								//���2��ǰһ������ǰ��1
	edge->halfEdge1 = halfEdge1;							//�±ߵ��������
	edge->halfEdge2 = halfEdge2;
	newVertex->SetCoord(point);								//�����µ�����

	//�µ������ĵ���
	CVertex* vtemp = vertex;								//�Ӹ������ҵ�����β,ͬʱ����µ�
	while (vtemp->nextv != nullptr) {
		vtemp = vtemp->nextv;
	}
	vtemp->nextv = newVertex;
	newVertex->prevv = vtemp;

	//�±߼�����ı���
	CSolid* solid = loop->lface->fsolid;					//�ҵ���������
	CEdge* eg = solid->sedge;
	if (eg == nullptr) {									//�����û�б�
		solid->sedge = edge;
	}
	else {													//������б�
		while (eg->nexte != nullptr) {
			eg = eg->nexte;
		}
		eg->nexte = edge;
		edge->preve = eg;
	}
	
	//����߼��뻷
	if (loop->ledge == nullptr) {							//�������û�а��
		loop->ledge = halfEdge1;							//���е�һ�������Ϊ���1
		halfEdge2->nxt = halfEdge1;							//���2����һ������ǰ��1
		halfEdge1->prv = halfEdge2;							//���1��ǰһ������ǰ��2
	}
	else {													//������д��ڰ��
		CHalfEdge* halfEdge = loop->ledge;					//�װ��
		while (halfEdge->nxt->hev != vertex) {				//�ҵ���һ����ߵ�����Ǹ�����İ��
			//printf("%d ", halfEdge->hev->vertexId);
			halfEdge = halfEdge->nxt;
		}
		//printf("44444444444\n");
		halfEdge2->nxt = halfEdge->nxt;						//���2����������Ϊ�ҵ��ߵ�����
		halfEdge->nxt = halfEdge1;							//�ҵ��ߵ�������Ϊ���1
		halfEdge1->prv = halfEdge;							//���1��ǰһ��������ҵ���
		halfEdge2->nxt->prv = halfEdge2;					//�ҵ��ߵ�ԭ�����ߵ�ǰ����Ϊ���2
	}

	return halfEdge1;
}

/// <summary>
/// �Ը���������Ϊ�˵������±�,�»�,����
/// v1->v2�İ�����ڻ�
/// </summary>
/// <param name="vertex1">β��</param>
/// <param name="vertex2">ͷ��</param>
/// <param name="loop"></param>
CLoop* mef(double* point1, double* point2, CLoop* loop)
{
	CVertex* vertex1 = loop->lface->fsolid->findVertex(point1);
	CVertex* vertex2 = loop->lface->fsolid->findVertex(point2);
	CHalfEdge* halfEdge1 = new CHalfEdge();					//�°��
	CHalfEdge* halfEdge2 = new CHalfEdge();
	CEdge* edge = new CEdge();								//�±�
	CLoop* newLoop = new CLoop();
	CFace* face = new CFace();

	//��߳�ʼ��
	halfEdge1->hee = halfEdge2->hee = edge;					//��߶�Ӧ��
	halfEdge1->hev = vertex1;								//���1�����Ϊv1
	halfEdge2->hev = vertex2;								//���2�����Ϊv2
	
	//����߼��뻷
	CHalfEdge* he1 = loop->ledge;
	CHalfEdge* he2 = loop->ledge;

	while (he1->hev != vertex1) {							//he1���Ϊv1�İ��
		he1 = he1->nxt;
	}
	while (he2->hev != vertex2) {							//he2���Ϊv2�İ��
		he2 = he2->nxt;
	}

	////////////////��Ҫ��ͼ���,��Ҳ��Ը�⿴�ڶ���,��дע����
	halfEdge1->prv = he1->prv;
	halfEdge1->nxt = he2;
	halfEdge2->prv = he2->prv;
	halfEdge2->nxt = he1;
	he1->prv->nxt = halfEdge1;
	he1->prv = halfEdge2;
	he2->prv->nxt = halfEdge2;
	he2->prv = halfEdge1;


	//�»���ʼ��
	newLoop->ledge = halfEdge1;
	newLoop->lface = face;

	//�»��еİ��ָ���»�
	while (halfEdge1->wloop != newLoop) {
		halfEdge1->wloop = newLoop;
		halfEdge1 = halfEdge1->nxt;
	}														//ѭ��������halfEdge1����ָ��ԭhalfEdge1

	loop->ledge = halfEdge2;								//�ɻ��İ����Ҫ���� �ɻ�Ϊv2->v1���ڻ�
	halfEdge2->wloop = loop;

	//�»���������Ļ���
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
	

	//�����ʼ��
	face->fsolid = loop->lface->fsolid;

	//��������������
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

	//�߳�ʼ��
	edge->halfEdge1 = halfEdge1;						//���ñ߶�Ӧ�İ��
	edge->halfEdge2 = halfEdge2;

	//�߼�����ı���
	CEdge* eg = loop->lface->fsolid->sedge;				//ָ������е��ױ�
	if (eg == nullptr) {								//���û�б�
		eg = edge;
	}
	else {												//������ڱ�
		while (eg->nexte != nullptr) {					//�ҵ�β��
			eg = eg->nexte;
		}
		eg->nexte = edge;								//���
		edge->preve = eg;
	}

	return newLoop;
}

/// <summary>
/// ɾ�����е��������,��һ����,ͬʱ����һ���ڻ�
/// </summary>
/// <param name="vertex1">һ����ߵ����</param>
/// <param name="vertex2">��һ����ߵ����,��Ϊ�ڻ���</param>
/// <param name="loop">����������ڵĻ�</param>
/// <returns>�ڻ�</returns>
CLoop* kemr(double* point1, double* point2, CLoop* loop)
{
	CHalfEdge* half = loop->ledge;
	CVertex* vertex1 = loop->lface->fsolid->findVertex(point1);
	CVertex* vertex2 = loop->lface->fsolid->findVertex(point2);
	CLoop* newLoop = new CLoop();								//���ڻ�
	CSolid* solid = loop->lface->fsolid;						//��������
	
	//ɾ�������������
	CHalfEdge* halfEdge1;										//v1->v2�İ��
	CHalfEdge* halfEdge2;										//v2->v1�İ��
	CHalfEdge* he = loop->ledge;
	while (he->hev != vertex1 || he->nxt->hev != vertex2) {		//�Ұ��1
		he = he->nxt;
	}
	halfEdge1 = he;
	he = he->nxt;
	while (he->hev != vertex2 || he->nxt->hev != vertex1) {		//�Ұ��2�����ڻ���
		he->wloop = newLoop;									//�ڻ��ϵİ��ָ���»�
		he = he->nxt;
	}
	halfEdge2 = he;

	//�ı��ڻ��ϰ�ߵ�ָ����⻷�ϰ�ߵ�ָ��
	halfEdge1->prv->nxt = halfEdge2->nxt;						//�⻷he1��ǰһ��
	halfEdge2->nxt->prv = halfEdge1->prv;						//�⻷he2�ĺ�һ��

	if (halfEdge1->nxt != halfEdge2) {							//�ڻ��ǿ� ,�ڻ�Ϊ��ʱ����Ҫ����
		halfEdge1->nxt->prv = halfEdge2->prv;					//�ڻ�he1�ĺ�һ��
		halfEdge2->prv->nxt = halfEdge1->nxt;					//�ڻ�he2��ǰһ��

		//�ڻ���ʼ��
		newLoop->ledge = halfEdge1->nxt;	
	}

	loop->ledge = halfEdge1->prv;
	
	newLoop->lface = loop->lface;

	//�ڻ�����������Ļ���
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

	//ɾ����ı��еĶ�Ӧ��
	CEdge* eg = solid->sedge;									
	while (eg != halfEdge1->hee) {
		eg = eg->nexte;
	}
	if (!eg->preve && eg->nexte) {									//egΪ�����ױ�������һ����
		solid->sedge = eg->nexte;
		solid->sedge->preve = nullptr;
	}
	else if(eg->preve && eg->nexte){								//������
		eg->preve->nexte = eg->nexte;
		eg->nexte->preve = eg->preve;
	}
	else if (eg->preve && !eg->nexte) {								//����β����ǰһ��
		eg->preve->nexte = nullptr;
	}
	else {															//ֻ��һ��
		solid->sedge = nullptr;
	}

	delete eg;
	delete halfEdge1;
	delete halfEdge2;
	
	return newLoop;
}

/// <summary>
/// ɾ��һ����,�����䶨��Ϊ��һ������ڻ�,��������������һ��ͨ�׻���������ϲ���һ������
/// </summary>
/// <param name="outLoop">�⻷</param>
/// <param name="inLoop">�ڻ�</param>
void kfmrh(CLoop* outLoop, CLoop* inLoop)
{
	CFace* inFace = inLoop->lface;
	inLoop->lface = outLoop->lface;
	//���ڻ����뵽�⻷������Ļ���
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

	//ɾ����
	if (inFace->prevf && inFace->nextf) {						//�������м�
		inFace->nextf->prevf = inFace->prevf;
		inFace->prevf->nextf = inFace->nextf;
	}
	else if (!inFace->prevf && inFace->nextf) {					//Ϊ���е�һ�����к�һ��
		outLoop->lface->fsolid->sface = inFace->nextf;
		inFace->nextf->prevf = nullptr;
	}
	else if (inFace->prevf && !inFace->nextf) {					//Ϊ�������һ������ǰһ��
		inFace->prevf->nextf = nullptr;
	}
	else {														//ֻ��һ����
		outLoop->lface->fsolid->sface = nullptr;
	}

	delete inFace;
}


/// <summary>
/// ɨ�Ӳ���
/// </summary>
/// <param name="face">����</param>
/// <param name="dx"></param>
/// <param name="dy"></param>
/// <param name="dz"></param>
void sweeping(CFace* face, double dx, double dy, double dz)
{
	CLoop* loop = face->floop;												//�⻷
	while (loop) {
		CHalfEdge* halfEdge = loop->ledge;
		CHalfEdge* he = loop->ledge;
		CHalfEdge* halfEdgePrv = halfEdge->prv;
		CHalfEdge* halfEdgeNxt = halfEdge->nxt;
		CLoop* loopNext = loop->nextl;

		double point1[3], point2[3], first[3], last[3];						//p1�ǵ������� p2��ƽ�ƺ������ first�ǻ���һ��������� last�ǻ���һ���������
		point1[0] = he->hev->VCoord[0];
		point1[1] = he->hev->VCoord[1];
		point1[2] = he->hev->VCoord[2];
		first[0] = last[0] = point2[0] = point1[0] + dx;
		first[1] = last[1] = point2[1] = point1[1] + dy;
		first[2] = last[2] = point2[2] = point1[2] + dz;
		mev(point1, point2, loop);											//p2���뻷
		
		while(he != halfEdgePrv) {
			he = halfEdgeNxt;
			halfEdgeNxt = halfEdgeNxt->nxt;
			point1[0] = he->hev->VCoord[0];
			point1[1] = he->hev->VCoord[1];
			point1[2] = he->hev->VCoord[2];
			point2[0] = point1[0] + dx;
			point2[1] = point1[1] + dy;
			point2[2] = point1[2] + dz;
			mev(point1, point2, loop);										//���뻷
			mef(point2, last, loop);										//������
			last[0] = point2[0];
			last[1] = point2[1];
			last[2] = point2[2];
		}
		mef(first, last, loop);												//��β
		loop = loopNext;
	}
}