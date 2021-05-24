#pragma once

#include "Event.h"
#include "../Missions/Mission Headers.h"


class Formulation_Event : public  Event {

	char type;  //subject to change into a code or somehting 
	
	int Target_Location; //in kilometers

	int Mission_Duration; //in days

	int Significance;


public:

	Formulation_Event(int ed, int id, char typ, int tloc, int mdur, int sig);

	


	bool Execute(MasterStation* MS);

};