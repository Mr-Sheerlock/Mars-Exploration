#include "P_Mission.h"

P_Mission::P_Mission(int Id, int FD, int TLOC, int Duration, int significance) : Mission(Id, FD, TLOC, Duration, significance)
{
	Assigned_Rover = nullptr;

}


void P_Mission::AssignRover(P_Rover* P) {

	if (Assigned_Rover) { return; }
	Assigned_Rover = P;
}

void P_Mission::UnAssignRover() {
	Assigned_Rover = nullptr;
}

Rover* P_Mission::GetAssignedRover() {

	return Assigned_Rover;

}