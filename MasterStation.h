#pragma once

#include "UI.h"
#include "Data Structures/Data Structure Headers.h"
#include "Missions/Mission Headers.h"
#include "Events/Event.h"
#include "Rovers/Rovers Headers.h"






class MasterStation {

	int N_Missions;
	int N_Rovers;
	int N_Mount, N_Polar, N_Emerg;
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
	
	PriorityQueue<Mission*> N_Execution_Missions;  //negative of Competion day

	///No need for the Completed Missions because once completed, the get outputted



public:
	MasterStation();
	
	//Data member getters
	PriorityQueue<E_Mission*>* ReturnWaitingEmerg();
	Queue<P_Mission*>* ReturnWaitingPolar();




	//////////Input///////
	void ReadInputFile();
	void ReadRovers();
	void ReadCheckupInfo();
	void ReadEvents();



	/////////Output///////
	void PrintEachDay();  
		
	void FinalOutput();


	/////////////Operation/////////////

	void ExecuteDay(); 

	//Notice that here, we need to scheme the order of operation so as to not miss anything

		//Proposed Order: Checkfail->  Events->Rovers Arrival->Rovers Checkup-> Rovers Maintainance->
		//->Missions Assignment


	void ExecuteEvent();


	
	void Checkfailed(); //loops on the in-execution missions and applies the probability and sees if the misson(s) has failed 

	//Idea: calculate the probability: 

	// after looping n times: the accumulative probability p 
	//                       p = (5%)^n

	// so instead we might use   5% = (x)^n   and calculate x in someway or another




	///////////////////////////////////////////
	void CheckRoverArrived();
	void CheckupComplete();
	void Maint_Complete();  //Maintainance Completion Check




	void AssignMission();

	void CheckMissionComplete();
	
	//prints the mission after its completion
	void PrintMission();


	void CalculateStats();




	~MasterStation();
};