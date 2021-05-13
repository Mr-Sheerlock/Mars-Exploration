#include "M_Mission.h"


int M_Mission::AutoP=10; //setting an initial value to the static member


M_Mission::M_Mission(int Id, int FD, int TLOC, int Duration, int significance):Mission( Id,  FD,  TLOC,  Duration, significance)
{
	MR=nullptr;
	ER=nullptr;
}


void M_Mission::SetAutoP(int AP)
{
	if (AP <= 0) { AutoP = 10; }
	AutoP = AP;
}

int   M_Mission::GetAutoP() {

	return AutoP;
}


void M_Mission::AssignM_Rover(M_Rover* R) {
	if (ER || MR ) { return; }   // if Rover is assigned 
		
	MR = R;
}

void M_Mission::AssignE_Rover(E_Rover* R) {
	if (ER || MR) { return; }   // if Rover is assigned 

	ER = R;
}

Rover* M_Mission::GetAssignedRover() { 
	if (ER) { return ER; }
	if (MR) { return MR; }
	else return nullptr;
	 }


void M_Mission::UnAssignRover() { ER = nullptr; MR = nullptr; }