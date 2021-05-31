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



void Mission::SetFormulationDay(int x) { Formulation_Day = x; }
int Mission::GetFormulationDay() { return Formulation_Day; }

void Mission::SetWaitingDays(int x) { Waiting_Days = x; }
int Mission::GetWaitingDays() { return Waiting_Days; }

void Mission::SetExecutionDays(int x) { Execution_Days = x; }
int Mission::GetExecutionDays() { return Execution_Days; }


void Mission::SetCompletionDay(int x) { Completion_Day = x; }
int Mission::GetCompletionDay() { return Completion_Day; }


void Mission::SetStartingDay(int x) { Starting_Day = x; }
int Mission::GetStartingDay() { return Starting_Day; }


void Mission::SetTYP(char x) { type = x; }
char Mission::GetTYP() { return type; }


void Mission::SetStatus(bool x ) { Status = x; }
bool Mission::GetStatus() { return Status; }



