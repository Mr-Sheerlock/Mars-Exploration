#include "P_Rover.h"

P_Rover::P_Rover(int id, float s) : Rover(id, s )
{
	
	SetType('P');

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

char P_Rover::GetType() {

	return 'P';

}

int P_Rover::CP=10;