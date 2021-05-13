#pragma once
#include"Mission.h"
class P_Mission : public Mission
{
	P_Rover* Assigned_Rover;

public:

	P_Mission(int Id, int FD, int TLOC, int Duration, int significance);
	
   void AssignRover(P_Rover* P);

   void UnAssignRover();

   Rover* GetAssignedRover();
};

