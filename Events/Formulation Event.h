#include "Event.h"


class Formulation_Event : Event {

	char type;  //subject to change into a code or somehting 
	int Target_Location; //in kilometers

	int Mission_Duration; //in days

	int Significance;

public:

	Formulation_Event(){}

	void Execute();

};