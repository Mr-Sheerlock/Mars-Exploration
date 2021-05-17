#pragma once

#include "Event.h"
#include "../Missions/Mission Headers.h"


class Formulation_Event : public  Event {

	char type;  //subject to change into a code or somehting 
	int Target_Location; //in kilometers

	int Mission_Duration; //in days

	int Significance;


public:

	Formulation_Event(int ed, int id, char typ, int tloc, int mdur, int sig ) : Event(ed, id)
	{
		type = typ;
		Target_Location = tloc;
		Mission_Duration = mdur;
		Significance = sig;
	}

	//should we make the return type of execute Mission pointer ? 
	bool Execute(MasterStation* MS) {

		
		if (type == 'P') {
			P_Mission* newMission = new P_Mission(getID(), getEventDay(), Target_Location, Mission_Duration, Significance);
			MS->ReturnWaitingPolar()->Enqueue(newMission);

		}
		else
			if (type == 'E') {
				int tempPriority=2; //temporarily 
				E_Mission* newMission = new E_Mission(getID(), getEventDay(), Target_Location, Mission_Duration, Significance, tempPriority);
				MS->ReturnWaitingEmerg()->Enqueue(newMission, tempPriority);

			}
		return true;
	}

};