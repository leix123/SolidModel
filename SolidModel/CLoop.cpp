#include "CLoop.h"

CLoop::CLoop()
{
	static int id = 0;
	loopId = id++;
	prevl = nextl = nullptr;
	ledge = nullptr;
	lface = nullptr;
}

CLoop::~CLoop()
{

}