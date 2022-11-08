#include "CFace.h"

CFace::CFace()
{
	static int id = 0;
	faceId = id++;
	prevf = nextf = nullptr;
	fsolid = nullptr;
	floop = nullptr;
}

CFace::~CFace() {

}