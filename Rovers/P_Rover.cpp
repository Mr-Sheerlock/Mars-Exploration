#include "P_Rover.h"

P_Rover::P_Rover(int id, float s) : Rover(id, s )
{
}

P_Rover::~P_Rover()
{
}

void P_Rover::Set_CheckupD(int CheckD)
{
	if (CheckD <= 0) { CP = 10; }
	CP = CheckD;
}

int P_Rover::GetCheckupD()
{
	return CP;
}

int P_Rover::CP=10;