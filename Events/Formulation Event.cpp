#include "Formulation Event.h"
#include "../MasterStation.h"
class MasterStation;

Formulation_Event::Formulation_Event(int ed, int id, char typ, int tloc, int mdur, int sig) : Event(ed, id) {

	type = typ;
	Target_Location = tloc;
	Mission_Duration = mdur;
	Significance = sig;
}


bool Formulation_Event::Execute(MasterStation* MS) {

	if (type == 'P') {
		P_Mission* newMission = new P_Mission(getID(), getEventDay(), Target_Location, Mission_Duration, Significance);
		MS->ReturnWaitingPolar()->Enqueue(newMission);
		MS->IncrementWaitingPolarCount();
	}
	else
		if (type == 'E') {
			int tempPriority = 2; //temporarily 
			E_Mission* newMission = new E_Mission(getID(), getEventDay(), Target_Location, Mission_Duration, Significance, tempPriority);
			MS->ReturnWaitingEmerg()->Enqueue(newMission, tempPriority);
			MS->IncrementWaitingEmerCount();

		}
	return true;
}

