#include "M_Mission.h"


int M_Mission::AutoP=10; //setting an initial value to the static member


M_Mission::M_Mission()
{
}

M_Mission::~M_Mission()
{
}

void M_Mission::setAutoP(int AP)
{
	if (AP <= 0) { AutoP = 10; }
	AutoP = AP;
}