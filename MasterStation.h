#pragma once

#include "UI.h"
#include "Data Structures/Data Structure Headers.h"
#include "Events/Event.h"
#include "Missions/Mission Headers.h"
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
	PriorityQueue<P_Rover*>* Av_P_Rovers; //available polar rovers
	PriorityQueue<E_Rover*>* Av_E_Rovers; //available emergency rovers

	PriorityQueue<E_Mission*>* Waiting_E_Missions; 
	Queue<P_Mission*>* Waiting_P_Missions;  

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

		//Proposed Order: Events->Rovers Arrival->Rovers Checkup-> Rovers Maintainance->
		//->Missions Assignment

	void ExecuteEvent();


	
	void Checkfailed(); //loops on the in-execution missions and applies the probability and sees if the misson(s) has failed 




	void CheckRoverArrived();



	///////////////////////////////////////////
	void CheckupComplete();


	void Maint_Complete();  //Maintainance Completion Check


	void AssignMission();




	void CalculateStats();




	~MasterStation();
};