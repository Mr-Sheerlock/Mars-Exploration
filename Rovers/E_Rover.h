#pragma once
#include "Rover.h"

class E_Rover : public Rover
{
public:
	E_Rover(int id, float s);
	~E_Rover();

	void static Set_CheckupD(int CheckD);
	int GetCheckupD();

private:
	int static CE; //checkup duration in days
};