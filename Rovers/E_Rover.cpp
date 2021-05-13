#include "E_Rover.h"

E_Rover::E_Rover(int id, float s) : Rover( id, s )
{
}

E_Rover::~E_Rover()
{
}

void E_Rover::Set_CheckupD(int CheckD)
{
	if (CheckD <= 0) { CE = 10; }
	CE = CheckD;
}

int E_Rover::GetCheckupD()
{
	return CE;
}

int E_Rover::CE=10;