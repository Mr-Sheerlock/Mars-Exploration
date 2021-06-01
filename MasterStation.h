#pragma once

#include "UI.h"
#include "Data Structures/Data Structure Headers.h"
#include "Missions/Mission Headers.h"
#include "Rovers/Rovers Headers.h"
class Event;





class MasterStation {
	
	int CurrentDay;


	//Stats
	int N_Missions;
	int N_Rovers;
	int NRoversP, NRoversE;
	int NMissionsP, NMissionsE;

	int WaitingMissionsP, WaitingMissionsE;

	int AvRoversE, AvRoversP;

	int CheckupRovers, CheckupRoversP;
	
	int MaintRovers, MaintRoversP;


	int NExecMiss;
	int NExecRovs;

	//Failure
	int ProbabilityOFfailure;


	//Others
	UI* IO_Interface;


	ifstream Input;
	ofstream Output;

	/////////////////////////////////////////Lists///////////////////////////////////////

	Queue<Event*>* EventList;

	PriorityQueue<P_Rover*>* Available_P_Rovers; //available polar rovers
	PriorityQueue<E_Rover*>* Available_E_Rovers; //available emergency rovers
	
	
	PriorityQueue<Rover*>* N_Execution_Rovers;   //negative Completion Day 
												 //or the day it will reach the station
												 //in case of failure

	PriorityQueue<Rover*>* Checkup_Rovers;       //negative Completion of Checkup Day 

	PriorityQueue<Rover*>* Maintainance_Rovers;  //negative Completion of Maint. Day

	
	Queue<P_Mission*>* Waiting_P_Missions;  

	PriorityQueue<E_Mission*>* Waiting_E_Missions; //Priority
	
	PriorityQueue<Mission*>* N_Execution_Missions;  //negative of Competion day

	///No need for the Completed Missions because once completed, the gets printed



	//Auxillary utilities
	double CalculateProbability(int Probability, int Duration);

public:
	MasterStation();
	

	//Events
	//Data member getters /incrementers
	PriorityQueue<E_Mission*>* ReturnWaitingEmerg();
	Queue<P_Mission*>* ReturnWaitingPolar();
	void IncrementWTP();
	void IncrementWTE();



	//////////Input///////
	void ReadInputFile();
	void ReadRovers();
	void ReadCheckupInfo();
	void ReadMaintDur();
	void ReadEvents();



	/////////Output///////
	void PrintEachDay();
	int TakeIdsFromWaitingE(Queue<int>& Ids);
	int TakeIdsFromWaitingP(Queue<int>& ids);
	int TakeInfoFromInExecution(Queue<int>& C, Queue<int>& M, Queue<int>& R, Queue<char>& RM, Queue<char>& Ctype, int& comp, int& Ecomp, int& E);
	int TakeIdsFromAvailableE(Queue<int>& id);
	int TakeIdsFromAvailableP(Queue<int>& id);
	int TakeInfoFromInCheckup(Queue<int>& id, Queue<char>& type, int& Ecount);
	template<typename T>
	void PutIdsInArray(Queue<T>& tempId, T*& ids, int size);
	void FinalOutput();	


	void PrintMission();

	/////////////Operations/////////////
	
	void ExecuteEvent(Queue<Event*>* EventList);

	void ExecuteDay(); 

	//Notice that here, we need to scheme the order of operation so as to not miss anything

		//Proposed Order: Checkfail->  Events->Rovers Arrival->Rovers Checkup-> Rovers Maintainance->
		//->Missions Assignment






	///////////////////////////////////////////
	void CheckRoverArrived();

	void UpdateRover(Rover*, Mission*); //update the data members of the rover

	bool CheckMaint(Rover*); //Check if a rover needs maintenance
	
	void Maint_Complete();  //Maintainance Completion Check

	void CheckupComplete();
	
	void DoesItNeedCheckUp(Rover* R); //checks if a rover needs checkup. If so, then its NeedCheckup Data member is set to true.
	void MoveFromCheckupToAvailable(Rover* R);
	
	void MoveFromInExecutionToCheckup(Rover* R);
	
	void MoveFromInExecutionToAvailable(Rover* R);

	void MoveFromInExecutionToMaintenance(Rover* R);


	void AssignMission();


	
	
	void CheckMissionComplete();
	
	//prints the mission after its completion
	//Failure
	
	void Checkfailed(); //loops on the in-execution missions and applies the probability and sees if the misson(s) has failed 

	//Idea: calculate the probability: 

	// after looping n times: the accumulative probability p 
	//                       p = (5%)^n

	// so instead we might use   5% = (x)^n   and calculate x in someway or another



	void CalculateStats();




	~MasterStation();
};