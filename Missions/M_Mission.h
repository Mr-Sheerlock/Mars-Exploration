#pragma once
#include"Mission.h"

class M_Mission : public Mission
{
	int static AutoP;

public:
	M_Mission();
	~M_Mission();
	void static setAutoP(int AP);



};

