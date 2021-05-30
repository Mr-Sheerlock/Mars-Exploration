#pragma once

#include "../Rovers/Rovers Headers.h"

//Abstract Class
class Mission
{
	int ID;

	int Formulation_Day;
	int Waiting_Days;
	int Execution_Days;

	int Starting_Day;

	int Completion_Day;

	int TargetLocation;
	
	int Significance;

	int MissionDuration; // The time needed to fulfill the mission at TARGET'S LOCATION

	char type;

	bool Status;     //0 for waiting/failed  , 1 for ongoing


	

public:
	
	Mission(int ID, int FD, int TLOC, int Duration, int Significance);

	void SetID(int x);
	int GetID();

	void SetDuration(int x);
	int GetDuration();

	virtual void SetTLOC(int x);
	int GetTLOC();

	void SetSIG(int x);
	int GetSIG();

	void SetFormulationDay(int x);  
	int GetFormulationDay();

	void SetWaitingDays(int x);   
	int GetWaitingDays();

	void SetExecutionDays(int x);    
	int GetExecutionDays();

	void SetCompletionDay(int x);   
	int GetCompletionDay();

	void SetTYP(char x);
	char GetTYP();

	void SetStatus(bool x);
	bool GetStatus();

	virtual Rover* GetAssignedRover()=0; //depends on each type
	
	virtual void UnAssignRover()=0;  //depends on each type



};

