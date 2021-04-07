#pragma once
#include <thread>
#include <atomic>
#include <string>
#include <ncurses.h>

enum class LaundryStatus {
	dirty,
	wet,
	clean
};

class Client
{
public:
	Client(WINDOW*, int, int, int, int, int);

	int getId();

	void display();
	void move(int, int);

	//float getWaitingTime();
	int getLaundryCounter();

	void start();
	void stop();

	void act();
	
	bool searchForFreeMachine();
	bool searchForFreeRack();
	
	void useWashingMachine();
	void useDryingRack();

	void getClothesDirty();

	void freeMachine();
	void freeRack();

private:
	WINDOW* window;
	int id;
	int x;
	int y;
	const int initX;
	const int initY;

	int occupiedId = 0;

	int laundryTime;
	int dryingTime;

	//float waitingTime = 0.0f;
	int laundryCounter = 0;

	std::atomic_bool isDoingLaundry;

	std::atomic<LaundryStatus> laundryStatus;

	std::unique_ptr<std::thread> lifeThread;
};

