#pragma once

#include "UI.h"
#include "Data Structures/Data Structure Headers.h"
#include "Missions/Mission Headers.h"
#include "Rovers/Rovers Headers.h"
class Event;





class MasterStation {
	
	int CurrentDay;


	//Stats
	//Missions
	int N_Missions;
	int  NMissionsE;
	
	int WaitingMissionsP, WaitingMissionsE;
	
	int NExecMiss, NExecMissE;
	
	//Rovers
	
	int  NRoversE, NRoversP;


	int AvRoversE, AvRoversP;

	int CheckupRovers, CheckupRoversE;
	
	int MaintRovers, MaintRoversE;


	int NExecRovs;

	int DailyCompletedCount,DailyCompletedCountE;

	int Failed_P_Rover, Failed_E_Rover;


	//Statistics
	float Total_Wait; //for use in statistics
	float Total_InExecution; //for use in statistics 

	//Failure
	int ProbabilityOFfailure;

	
	//Output
	int* DailyCompMissionsIDs;  //used to collect the completed missions ID's for the Output
	char* DailyCompMissionsType;

	int UserChoice; //1 for Interactive, 2 for Step-By-Step, 3 for Silent

	bool SilentModeFlag;

	//Others
	UI* IO_Interface;


	ifstream Input;
	ofstream Output;

	////////////////////////Lists///////////////////////////////////////

	Queue<Event*>* EventList;

	PriorityQueue<P_Rover*>* Available_P_Rovers; //available polar rovers
	PriorityQueue<E_Rover*>* Available_E_Rovers; //available emergency rovers
	
	Queue<P_Mission*>* Waiting_P_Missions;  

	PriorityQueue<E_Mission*>* Waiting_E_Missions; //Priority
	
	PriorityQueue<Rover*>* N_Execution_Rovers;   //negative Completion Day 
												 //or the day it will reach the station
												 //in case of failure

	PriorityQueue<Mission*>* N_Execution_Missions;  //negative of Competion day

	PriorityQueue<Rover*>* Checkup_Rovers;       //negative Completion of Checkup Day 

	PriorityQueue<Rover*>* Maintainance_Rovers;  //negative Completion of Maint. Day

	
	

	///No need for the Completed Missions because once completed, the gets printed



	//Auxillary utilities
	double CalculateProbability(int Probability, int Duration);

public:
	MasterStation();
	

	//Events
	//Data member getters /incrementers
	PriorityQueue<E_Mission*>* ReturnWaitingEmerg();
	Queue<P_Mission*>* ReturnWaitingPolar();
	void IncrementWaitingPolarCount();
	void IncrementWaitingEmerCount();

	int Get_N_RoversP();


	//////////Input///////
	void ReadInputFile();
	void ReadRovers();
	void ReadCheckupInfo();
	void ReadMaintDur();
	void ReadEvents();



	/////////Output///////
	void PrintEachDay();
	void TakeIdsFromWaitingE(int* Ids);
	void TakeIdsFromWaitingP(int* ids);
	void TakeInfoFromInExecution(int* M, int* R, char* RM, int* FR, char* FT);
	void TakeFailedFromInExecRov(int* FR, char* FT);
	void TakeIdsFromAvailableE(int* id);
	void TakeIdsFromAvailableP(int* id);
	void TakeInfoFromInCheckup(int* id, char* type);
	void TakeInfoFromInMaint(int* id, char* type);
	
	void FinalOutput();	

	void ReadUserChoice();

	///Checks if the Numbers are consistent or not
	bool CheckConsistency();

	/////////////Operations/////////////
	
	void ExecuteEvents();

	void ExecuteDay(); 

	bool CheckLastDay();
	
	//Mission assignment
	void AssignMission();
	void CalculateArrive2Target(Rover* rover, int Tloc);
	bool GetRoverFromMaintenance(Rover*& RoverNeeded, char Type);
	void SearchForPolar(P_Rover*& NeededPRover, Queue<Rover*>*& TempQ);
	void SearchForPolar_Emergency(E_Rover*& NeededERover, P_Rover*& NeededPRover, Queue<Rover*>*& TempQ);
	void ReturnToMaint(Queue<Rover*>* TempQ);
	void CheckMissionComplete();

	void ReturnToMaint_FindRover(Rover* RoverToFind, Queue<Rover*>* TempQ);

	///////////////////////////////////////////
	void CheckRoverArrived();

	void UpdateRover(Rover*, Mission*); //update the data members of the rover

	bool CheckMaint(Rover*); //Check if a rover needs maintenance
	
	void Maint_Complete();  //Maintainance Completion Check

	void CheckupComplete();
	
	void DoesItNeedCheckUp(Rover* R); //checks if a rover needs checkup. If so, then its NeedCheckup Data member is set to true.

	//Movers
	void MoveFromCheckupToAvailable(Rover* R);
	
	void MoveFromInExecutionToCheckup(Rover* R);
	
	void MoveFromInExecutionToAvailable(Rover* R);

	void MoveFromInExecutionToMaintenance(Rover* R);

	void MoveFromWaitingToInExecution(Mission* mission, Rover* rover);


	
	//prints the mission after its completion
	//Failure
	
	void Checkfailed(); //loops on the in-execution missions and applies the probability and sees if the misson(s) has failed 

	//Idea: calculate the probability: 

	// after looping n times: the accumulative probability p 
	//                       p = (5%)^n

	// so instead we might use   5% = (x)^n   and calculate x in someway or another



	void CalculateStats(float& AvgWait, float& AvgExec);





	~MasterStation();
};