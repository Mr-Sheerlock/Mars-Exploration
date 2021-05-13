#pragma once

#include "UI.h"

#include "Data Structures/Data Structure Headers.h"
#include "Events/Event Headers.h"
#include "Missions/Mission Headers.h"
#include "Rovers/Rovers Headers.h"






class MasterStation {

	int N_Missions;
	int N_Rovers;
	int N_Mount, N_Polar, N_Emerg;
	UI* IO_Interface;
	ifstream Input;


	/////////////////////////////////////////Lists///////////////////////////////////////

	Queue<Event*>* EventList;
	PriorityQueue<M_Rover*>* Av_M_Rovers; //available mountainous rovers
	PriorityQueue<P_Rover*>* Av_P_Rovers; //available polar rovers
	PriorityQueue<E_Rover*>* Av_E_Rovers; //available emergency rovers


public:
	MasterStation();
	

	//////////Input///////
	void ReadInputFile();
	void ReadRovers();
	void ReadCheckupInfo();
	void ReadAutoP();
	void ReadEvents();
	void ExecuteDay(); 



	/////////Output///////
	void PrintDay();
	void FinalOutput();






	~MasterStation();
};