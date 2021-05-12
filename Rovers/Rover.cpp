#include "Rover.h"

Rover::Rover(int id, int s)
{
	ID = id;
	Speed = s;
}

Rover::~Rover()
{
}

void Rover::SetSpeed(int s)
{
	Speed = s;
}

int Rover::GetSpeed()
{
	return Speed;
}


int Rover::GetID()
{
	return ID;
}

void Rover::setN(int n)
{
	N = n;
}

int Rover::N = 0; //set an initial value