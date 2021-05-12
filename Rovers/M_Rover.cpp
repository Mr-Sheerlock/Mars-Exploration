#include "M_Rover.h"

M_Rover::M_Rover(int id, float s) : Rover{ id ,s }
{
}

M_Rover::~M_Rover()
{
}

void M_Rover::Set_CheckupD(int CheckD)
{
	CM = CheckD;
}

int M_Rover::GetCheckupD()
{
	return CM;
}

int M_Rover::CM = 0;