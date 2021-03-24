#pragma once
#include <memory>
#include <thread>
#include <atomic>
#include <string>
#include <ncurses.h>

class ServiceWorker
{
public:
	ServiceWorker(WINDOW*, int, int);

	void display();
	void move(int, int);
	
	void start();
	void stop();

	void act();

	void searchForBrokenMachines();

	void fixBrokenMachine();

private:
	WINDOW* window;
	int x;
	int y;

	int machineId = 0;

	int laundryTime;
	int dryingTime;

	float waitingTime = 0.0f;

	std::atomic_bool isWorking;

	std::unique_ptr<std::thread> lifeThread;
};

