#define _CRT_SECURE_NO_DEPRECATE
#include<Gl/glut.h>
#include <cstdio>
#include"Euler.h"
	
void print(CSolid* solid)										//调试用
{
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	CFace* face = solid->sface;
	int index = 0;
	while (face){												//循环面
		printf("faceID:%d \n", face->faceId);
		CLoop* temploop = face->floop;
		while (temploop){										//循环环
			CHalfEdge* halfedge = temploop->ledge;
			printf("start:%d   ", halfedge->hev->vertexId);
			CHalfEdge* tmp = halfedge->nxt;
			while (tmp != halfedge){							//循环点
				printf("%d   ", tmp->hev->vertexId);
				tmp = tmp->nxt;
			}
			printf("\n");
			temploop = temploop->nextl;
		}
		face = face->nextf;
		index++;
	}
	printf("--------------------------------------------------------------------------\n");
}


GLUtesselator* obj = gluNewTess();			//产生一个网格化变量
int color_faces[300][3];					//存储随机产生的各面颜色

CSolid* solid = new CSolid;
//绕x，y，z轴旋转，分别按上下左右方向键和上下翻页键
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = 0.0f;

void CALLBACK beginCallback(GLenum which)
{
	glBegin(which);
}
void CALLBACK errorCallback(GLenum errorCode)
{
	const GLubyte* estring;
	estring = gluErrorString(errorCode);
	fprintf(stderr, "Tessellation Error: %s\n", estring);
	exit(0);
}
void CALLBACK endCallback(void)
{
	glEnd();
}
void CALLBACK vertexCallback(GLvoid* vertex)
{
	const GLdouble* pointer;
	pointer = (GLdouble*)vertex;
	glVertex3dv(pointer);
}

void ChangeSize(int w, int h)
{
	if (h == 0) h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat nRange = 150.0f;

	if (w <= h)
		glOrtho(-nRange, nRange, -nRange * h / w, nRange * h / w, -nRange, nRange);
	else
		glOrtho(-nRange * w / h, nRange * w / h, -nRange, nRange, -nRange, nRange);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/// <summary>
/// 画图
/// </summary>
void show()
{
	CFace* face = solid->sface;
	int index = 0;
	while (face){
		gluTessBeginPolygon(obj, NULL);
		CLoop* temploop = face->floop;

		unsigned char red = color_faces[index][0];
		unsigned char green = color_faces[index][1];
		unsigned char blue = color_faces[index][2];
		while (temploop){
			glColor3b(red, green, blue);
			CHalfEdge* halfedge = temploop->ledge;
			CVertex* start = halfedge->hev;
			gluTessBeginContour(obj);
			gluTessVertex(obj, start->VCoord, start->VCoord);
			CVertex* tmp = halfedge->nxt->hev;
			halfedge = halfedge->nxt;
			while (tmp != start){
				gluTessVertex(obj, tmp->VCoord, tmp->VCoord);
				halfedge = halfedge->nxt;
				tmp = halfedge->hev;
			}
			gluTessEndContour(obj);
			temploop = temploop->nextl;
		}
		gluTessEndPolygon(obj);
		face = face->nextf;
		index++;
	}
}

/// <summary>
/// 初始化
/// </summary>
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	gluTessCallback(obj, GLU_TESS_VERTEX, (void (CALLBACK*)())vertexCallback);
	gluTessCallback(obj, GLU_TESS_BEGIN, (void (CALLBACK*)())beginCallback);
	gluTessCallback(obj, GLU_TESS_END, (void (CALLBACK*)())endCallback);
	gluTessCallback(obj, GLU_TESS_ERROR, (void (CALLBACK*)())errorCallback);
}

/// <summary>
/// 使用五个欧拉操作建实体模型
/// </summary>
/// <returns></returns>
CSolid* makeSolid()
{
	//实心立方体坐标
	//double points[][3] = { { 0.0, 0.0, 0.0 }, { 60.0, 0.0, 0.0 }, { 60.0, 60.0, 0.0 }, { 0.0, 60.0, 0.0 },
	//{ 0.0, 0.0, 60.0 }, { 60.0, 0.0, 60.0 }, { 60.0, 60.0, 60.0 }, { 0.0, 60.0, 60.0 } };

	//带孔立方体坐标
	double points[][3] = { { 0.0, 0.0, 0.0 }, { 60.0, 0.0, 0.0 }, { 60.0, 120.0, 0.0 }, { 0.0, 120.0, 0.0 },
		{ 0.0, 0.0, 30.0 }, { 60.0, 0.0, 30.0 }, { 60.0, 120.0, 30.0 }, { 0.0, 120.0, 30.0 },
		{ 10.0, 40.0, 30.0 }, { 50.0, 40.0, 30.0 }, { 50.0, 80.0, 30.0 }, { 10.0, 80.0, 30.0 },
		{ 10.0, 40.0, 0.0 }, { 50.0, 40.0, 0.0 }, { 50.0, 80.0, 0.0 }, { 10.0, 80.0, 0.0 }
	};

	CSolid* solid = mvsf(points[0]);
	CLoop* loop = solid->sface->floop;
	
	mev(points[0], points[1], loop);
	
	mev(points[1], points[2], loop);
	mev(points[2], points[3], loop);
	CLoop* loopTemp = mef(points[3], points[0], loop);
	
	mev(points[0], points[4], loop);
	mev(points[1], points[5], loop);
	
	mef(points[4], points[5], loop); 
	mev(points[2], points[6], loop);
	mef(points[5], points[6], loop);
	mev(points[3], points[7], loop);
	mef(points[6], points[7], loop);
	mef(points[7], points[4], loop);

	mev(points[4], points[8], loop);

	mev(points[8], points[9], loop);
	mev(points[9], points[10], loop);
	mev(points[10], points[11], loop);

	CLoop* newLoop = mef(points[11], points[8], loop); 
	kemr(points[4], points[8], loop);        //newloop为已加入环列的内环，loop为上表面的外环
	
	mev(points[8], points[12], newLoop);
	mev(points[9], points[13], newLoop);
	mef(points[13], points[12], newLoop);

	mev(points[10], points[14], newLoop); 
	mef(points[14], points[13], newLoop);
	
	mev(points[11], points[15], newLoop);
	
	mef(points[15], points[14], newLoop);
	CLoop* loopTemp2 = mef(points[15], points[12], newLoop);

	kfmrh(loopTemp, loopTemp2);

	return solid;
}

/// <summary>
/// 通过扫掠操作建立实体模型
/// </summary>
/// <returns></returns>
CSolid* sweepingSolid()
{
	//底面坐标
	double points[][3] = { { 0.0, 0.0, 0.0 }, { 60.0, 0.0, 0.0 }, { 60.0, 120.0, 0.0 }, { 0.0, 120.0, 0.0 },
		{ 10.0, 40.0, 0.0 }, { 50.0, 40.0, 0.0 }, { 50.0, 80.0, 0.0 }, { 10.0, 80.0, 0.0 }};

	//绘制底面
	CSolid* solid = mvsf(points[0]);
	CLoop* loop = solid->sface->floop;

	mev(points[0], points[1], loop);

	mev(points[1], points[2], loop);
	mev(points[2], points[3], loop);
	CLoop* outLoop = mef(points[3], points[0], loop);

	mev(points[0], points[4], loop);

	mev(points[4], points[5], loop);
	mev(points[5], points[6], loop);
	mev(points[6], points[7], loop);

	CLoop* inLoop = mef(points[7], points[4], loop);

	kfmrh(outLoop, inLoop);

	kemr(points[0], points[4], loop);
	sweeping(loop->lface, 0, 0, 50);

	return solid;
}


/// <summary>
/// 读文件底面信息通过扫掠操作建实体模型
/// </summary>
/// <returns></returns>
CSolid* readFileSweepingSolid()
{
	CSolid* solid;
	FILE* file;
	file = fopen("input.txt", "r");
	int loopNum;												//环的数量
	fscanf(file, "%d\n", &loopNum);
	int loopPointsNum[10];										//每个环的点数量
	for (int i = 0; i < loopNum; i++) {
		fscanf(file, "%d", &loopPointsNum[i]);
	}

	double points[50][3];										//点坐标

	int pointIndex = 1;

	//读取外环
	for (int i = 0; i < 3; i++) {
		fscanf(file, "%lf", &points[0][i]);
	}

	solid = mvsf(points[0]);

	CLoop* loop = solid->sface->floop;
	for (int i = 1; i < loopPointsNum[0]; i++) {
		fscanf(file, "%lf %lf %lf", &points[pointIndex][0], &points[pointIndex][1], &points[pointIndex][2]);
		mev(points[pointIndex - 1], points[pointIndex], loop);
		pointIndex++;
	}
	CLoop* outLoop = mef(points[pointIndex-1], points[0], loop);

	//读取内环
	for (int i = 0; i < loopNum - 1; i++) {
		//第一个点
		fscanf(file, "%lf %lf %lf", &points[pointIndex][0], &points[pointIndex][1], &points[pointIndex][2]);
		mev(points[0], points[pointIndex], loop);
		pointIndex++;

		//后面的点
		for (int j = 1; j < loopPointsNum[i + 1]; j++) {
			fscanf(file, "%lf %lf %lf", &points[pointIndex][0], &points[pointIndex][1], &points[pointIndex][2]);
			mev(points[pointIndex - 1], points[pointIndex], loop);
			pointIndex++;
		}
		CLoop* inLoop = mef(points[pointIndex-1], points[pointIndex-loopPointsNum[i+1]], loop);
		kfmrh(outLoop, inLoop);

		kemr(points[0], points[pointIndex - loopPointsNum[i + 1]], loop);
	}

	double dx, dy, dz;										//矢量
	fscanf(file, "%lf %lf %lf", &dx, &dy, &dz);

	sweeping(loop->lface, dx, dy, dz);

	fclose(file);
	return solid;
}


void mySolid()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				//清除颜色及深度缓冲区
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	//旋转图形
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	
	glFrontFace(GL_CCW);											//指定逆时针绕法的多边形为多边形正面
	show();															//画图
	glPopMatrix();
	glFlush();
	
	glutSwapBuffers();												//刷新命令缓冲区
}

/// <summary>
/// 键盘操作
/// </summary>
/// <param name="key"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void keysFunc(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) xRot -= 5.0f;
	if (key == GLUT_KEY_DOWN) xRot += 5.0f;
	if (key == GLUT_KEY_LEFT) yRot -= 5.0f;
	if (key == GLUT_KEY_RIGHT) yRot += 5.0f;
	if (key == GLUT_KEY_PAGE_UP) zRot -= 5.0f;
	if (key == GLUT_KEY_PAGE_DOWN) zRot += 5.0f;

	if (xRot > 356.0f) xRot = 0.0f;
	if (xRot < -1.0f) xRot = 355.0f;
	if (yRot > 356.0f) yRot = 0.0f;
	if (yRot < -1.0f) yRot = 355.0f;
	if (zRot > 356.0f) zRot = 0.0f;
	if (zRot < -1.0f) zRot = 355.0f;

	//刷新窗口
	glutPostRedisplay();
}

void menuFunc(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	switch (value)
	{
	case 1:
		solid = makeSolid();
		break;
	case 2:
		solid = sweepingSolid();
		break;
	case 3:
		solid = readFileSweepingSolid();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	solid = makeSolid();						//通过欧拉操作make solid
	//solid = sweepingSolid();					//通过扫掠操作make solid
	//solid = readFileSweepingSolid();			//读取文件通过扫掠操作make solid
	srand(1);
	for (int i = 0; i < 100; i++) {
		unsigned char red = rand() % 255;
		unsigned char green = rand() % 255;
		unsigned char blue = rand() % 255;
		color_faces[i][0] = red;
		color_faces[i][1] = green;
		color_faces[i][2] = blue;
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(1000, 800);
	glutCreateWindow("Display Solid");

	init();
	glutReshapeFunc(ChangeSize);				//处理窗口改变
	glutSpecialFunc(keysFunc);					//处理键盘上下左右
	glutDisplayFunc(mySolid);					//处理绘图
	//创建右键菜单
	glutCreateMenu(menuFunc);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutAddMenuEntry("solid demo", 1);
	glutAddMenuEntry("sweeping solid demo", 2);
	glutAddMenuEntry("read file", 3);
	glutMainLoop();
	
	system("pause");
	return 0;
}