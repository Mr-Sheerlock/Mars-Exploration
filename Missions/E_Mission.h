#pragma once
#include"Mission.h"

class E_Mission : public Mission
{

	int MissionPriority;     //Priority assigned according to priority Eqn

	//Pvalue= Formulation Day, Target Location, Mission Duration, Significance 
	
	// P =  0.3 *TLOC + 0.2 *MD + 0.5*SIG - 0.2 * FD   

	//or (FD% AvgFD)
	
	// = Sig*(2* TLOC+ 5*MDUR) -2*FD 


	Rover* Assigned_Rover;   

public: 
	
	E_Mission(int Id, int FD, int TLOC, int Duration, int significance);

	void SetPriority();

	int  GetPriority();

	void AssignRover(Rover*);
	void UnAssignRover();

	Rover* GetAssignedRover();
	

};

