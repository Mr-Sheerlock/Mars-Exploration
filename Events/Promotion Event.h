#include "Event.h"


class Promotion_Event : Event {




public:

	Promotion_Event(int ed, int id) : Event ( ed, id )
	{
	}

	void Execute();
};