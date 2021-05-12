#include"Event.h"

class Cancel_Event : Event {
	
	
public:
	
	Cancel_Event(int ed, int id): Event (ed, id){}
	~Cancel_Event();

	void Execute() {


			
	}
};