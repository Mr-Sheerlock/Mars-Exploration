#include "Formulation Event.h"


Formulation_Event::Formulation_Event(int ed, int id, char typ, int tloc, int mdur, int sig) : Event(ed, id) {

	type = typ;
	Target_Location = tloc;
	Mission_Duration = mdur;
	Significance = sig;
}

//Code Produces no error but needs Testing 

bool Formulation_Event::Execute(MasterStation* MS) {

	if (type == 'P') {
		P_Mission* newMission = new P_Mission(getID(), getEventDay(), Target_Location, Mission_Duration, Significance);
		MS->ReturnWaitingPolar()->Enqueue(newMission);

	}
	else
		if (type == 'E') {
			int tempPriority = 2; //temporarily 
			E_Mission* newMission = new E_Mission(getID(), getEventDay(), Target_Location, Mission_Duration, Significance, tempPriority);
			MS->ReturnWaitingEmerg()->Enqueue(newMission, tempPriority);

		}
	return true;
}

