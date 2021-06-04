#include "E_Mission.h"


E_Mission::E_Mission(int Id, int FD, int TLOC, int Duration, int significance) : Mission(Id, FD, TLOC, Duration, significance) {

	SetTYP('E');
	Assigned_Rover = nullptr;

	SetPriority();
}

void E_Mission::SetPriority() {
	MissionPriority = GetTLOC() + 5* GetDuration() + 100* GetSIG() - GetFormulationDay()  ;

	// P =  1 *TLOC + 5 *MD + 100*SIG - 1 * FD   
	
	
	
	
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