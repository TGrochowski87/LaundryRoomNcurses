#include "DryingRack.h"

DryingRack::DryingRack(int x, int y)
{
	this->x = x;
	this->y = y;
}

int DryingRack::getX()
{
	return this->x;
}

int DryingRack::getY()
{
	return this->y;
}

void DryingRack::setX(int x)
{
	this->x = x;
}

void DryingRack::setY(int y)
{
	this->y = y;
}

bool DryingRack::useIfAvailable()
{
	std::lock_guard<std::mutex> lock(mtx);
	if (this->available) {
		this->available = false;
		return true;
	}
	else {
		return false;
	}
}

void DryingRack::setAvailability(bool set)
{
	std::lock_guard<std::mutex> lock(mtx);
	this->available = set;
}
