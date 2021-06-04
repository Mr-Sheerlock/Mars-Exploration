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
		if (MS->Get_N_RoversP() == 0) {

			return false;
		}

		P_Mission* newMission = new P_Mission(getID(), getEventDay(), Target_Location, Mission_Duration, Significance);
		MS->ReturnWaitingPolar()->Enqueue(newMission);
		MS->IncrementWaitingPolarCount();
	}
	else
		if (type == 'E') {

			E_Mission* newMission = new E_Mission(getID(), getEventDay(), Target_Location, Mission_Duration, Significance);
			MS->ReturnWaitingEmerg()->Enqueue(newMission, newMission->GetPriority());
			MS->IncrementWaitingEmerCount();

		}
	return true;
}

