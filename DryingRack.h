#pragma once
#include <mutex>

class DryingRack
{
public:
	DryingRack(int, int);

	bool useIfAvailable();
	void setAvailability(bool);

	int getX();
	int getY();

	void setX(int);
	void setY(int);

private:
	int id;
	int x;
	int y;

	bool available = true;

	std::mutex mtx;
};

