#include "ServiceWorker.h"

#include <chrono>

#include "Equipment.h"
#include "WashingMachine.h"

ServiceWorker::ServiceWorker(WINDOW* win, int x, int y)
	: isWorking{true}
{
	this->window = win;
	this->x = x;
	this->y = y;
}

void ServiceWorker::display()
{
	mvwaddch(window, y, x, 'S');
}

void ServiceWorker::move(int newX, int newY)
{
	mvwaddch(window, y, x, ' ');
	x = newX;
	y = newY;
}

void ServiceWorker::start()
{
	lifeThread = std::make_unique<std::thread>([this]() {act(); });
}

void ServiceWorker::stop()
{
	isWorking.store(false);
	lifeThread->join();
}

void ServiceWorker::act()
{
	while (isWorking.load())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		searchForBrokenMachines();
	}
}

void ServiceWorker::searchForBrokenMachines()
{
	for (int i = 0; i < Equipment::getWashingMachines().size(); i++)
	{
		if (!Equipment::getWashingMachines().at(i)->isFunctional())
		{
			machineId = i;
			fixBrokenMachine();
			return;
		}
	}
	move(1, 2);
	return;
}

void ServiceWorker::fixBrokenMachine()
{
	int machineX = Equipment::getWashingMachines().at(machineId)->getX();
	int machineY = Equipment::getWashingMachines().at(machineId)->getY();
	move(machineX - 1, machineY);
	Equipment::getWashingMachines().at(machineId)->repairMachine();
}
