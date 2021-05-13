#pragma once
#include"Mission.h"

class E_Mission : public Mission
{

	int MissionPriority;     //Priority assigned according to priority Eqn

	Rover* Assigned_Rover;   

public: 
	
	E_Mission(int Id, int FD, int TLOC, int Duration, int significance, int priority);

	void SetPriority(int P);

	int  GetPriority();

	void AssignRover(Rover*);
	void UnAssignRover();

	Rover* GetAssignedRover();
	

};

