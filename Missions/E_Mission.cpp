#include "E_Mission.h"


E_Mission::E_Mission(int Id, int FD, int TLOC, int Duration, int significance, int priority) : Mission(Id, FD, TLOC, Duration, significance) {

	SetTYP('E');
	Assigned_Rover = nullptr;

	SetPriority(priority);
}

void E_Mission::SetPriority(int P) {
	MissionPriority = P;
	// P =  0.3 *TLOC + 0.2 *MD + 0.5*SIG - 0.1 * FD   
	
	//or (FD% AvgFD)

}

int  E_Mission::GetPriority() { return MissionPriority; }

void E_Mission::AssignRover(Rover* R){
	if (Assigned_Rover) { return; } // if it's already assigned

	Assigned_Rover = R;
}


void E_Mission::UnAssignRover() {

	Assigned_Rover = nullptr;
}

Rover* E_Mission::GetAssignedRover() {

	return Assigned_Rover;
}