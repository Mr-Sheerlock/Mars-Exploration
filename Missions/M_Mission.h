#pragma once
#include"Mission.h"

class M_Mission : public Mission
{
	int static AutoP;

	M_Rover* MR;

	E_Rover* ER;    // I made 2 pointers to ensure nothing of type polar will get assigned
					// putting in mind that at no point should both of the pointers be 
					// assigned at the same time.


public:
	M_Mission(int Id, int FD, int TLOC, int Duration, int significance);

	
	void static SetAutoP(int AP);
	int static GetAutoP();

	void AssignM_Rover(M_Rover*);

	void AssignE_Rover(E_Rover*);

	void UnAssignRover();

	Rover* GetAssignedRover();

};

