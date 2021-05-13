#include "Mission.h"

Mission::Mission(int Id, int FD, int TLOC, int Duration, int significance) {

	ID = Id;

	Formulation_Day = FD;

	TargetLocation = TLOC;

	MissionDuration = Duration;

	Significance = significance;


	Waiting_Days =0 ;  //calculated by (FD-Current Day) 
	
	//Might possibly need TotalWaiting days member if it's in the output
	
	Status=0;


}



void Mission::SetID(int x) { ID = x; }
int Mission::GetID() { return ID; }





void Mission::SetDuration(int x) { MissionDuration = x; }
int Mission::GetDuration() { return MissionDuration; }

void Mission::SetTLOC(int x) { TargetLocation = x; }
int Mission::GetTLOC() { return TargetLocation; }

void Mission::SetSIG(int x) { Significance = x; }
int Mission::GetSIG() { return Significance; }



void Mission::SetFD(int x) { Formulation_Day = x; }
int Mission::GetFD() { return Formulation_Day; }

void Mission::SetWD(int x) { Waiting_Days = x; }
int Mission::GetWD() { return Waiting_Days; }

void Mission::SetED(int x) { Execution_Days = x; }
int Mission::GetED() { return Execution_Days; }


void Mission::SetCD(int x) { Completion_Day = x; }
int Mission::GetCD() { return Completion_Day; }


void Mission::SetTYP(char x) { type = x; }
char Mission::GetTYP() { return type; }


void Mission::SetStatus(bool x ) { Status = x; }
bool Mission::GetStatus() { return Status; }



