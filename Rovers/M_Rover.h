#pragma once
#include "Rover.h"

class M_Rover : public Rover
{
public:
	M_Rover(int id, float s);
	~M_Rover();
	void static Set_CheckupD(int CheckD);
	int GetCheckupD();

private:
	int static CM; //checkup duration in days
};
