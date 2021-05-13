#pragma once

#include "Event.h"
#include "../Missions/Mission Headers.h"


class Formulation_Event : public  Event {

	char type;  //subject to change into a code or somehting 
	int Target_Location; //in kilometers

	int Mission_Duration; //in days

	int Significance;

public:

	Formulation_Event(int ed, int id, char typ, int tloc, int mdur, int sig) : Event (ed,id )
	{
		type = typ;
		Target_Location = tloc;
		Mission_Duration = mdur;
		Significance = sig;
	}

	//should we make the return type of execute Mission pointer ? 
	void Execute() {
		Mission* newMission;
		/*if(type== 'M'){
			newMission = new M_Mission;
		}
		else 
		if (type=='P'){
			newMission = new P_Mission;
		
		}
		else 
		if (type == 'E') {

			newMission = new E_Mission();
		}*/

		

	}

};