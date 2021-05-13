#pragma once
#include "Rover.h"

class P_Rover : public Rover
{
public:
	P_Rover(int id, float s);
	~P_Rover();
	void static Set_CheckupD(int CheckD);
	static int GetCheckupD();

private:
	int static CP; //checkup duration in days

};