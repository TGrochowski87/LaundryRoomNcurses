#pragma once
#include <mutex>
#include <ncurses.h>

class DryingRack
{
public:
	DryingRack(WINDOW*, int, int);

	bool useIfAvailable();
	void setAvailability(bool);

	int getX();
	int getY();

	void display();

private:
	WINDOW* window;
	int id;
	int x;
	int y;

	bool available = true;

	std::mutex mtx;
};

