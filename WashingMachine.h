#pragma once
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <ncurses.h>

class WashingMachine
{
public:
	WashingMachine(WINDOW*, int, int);

	int getX();
	int getY();

	void setX(int);
	void setY(int);

	void display();

	bool useIfAvailable(int);
	void setAvailability(bool, int);

	void repairMachine();
	void setFunctional(bool);
	bool isFunctional();

private:
	WINDOW* window;
	int x;
	int y;

	std::atomic_bool functional;
	bool available = true;

	std::mutex mtx;
	std::condition_variable cv;
};

