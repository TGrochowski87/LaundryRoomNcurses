#include "ServiceWorker.h"

#include <chrono>
#include<iostream>

#include "Equipment.h"
#include "WashingMachine.h"

ServiceWorker::ServiceWorker(WINDOW* win, int x, int y)
	: isWorking{true}
{
	this->window = win;
	this->x = x;
	this->y = y;
}

int ServiceWorker::getX()
{
	return this->x;
}

int ServiceWorker::getY()
{
	return this->y;
}

void ServiceWorker::setX(int x)
{
	this->x = x;
}

void ServiceWorker::setY(int y)
{
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
		//std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		searchForBrokenMachines();
	}
}

std::string ServiceWorker::getStatus()
{
	return status;
}

void ServiceWorker::searchForBrokenMachines()
{
	//std::cout << "SERVICE WORKER " << __FUNCSIG__ << " 1" << std::endl;
	for (int i = 0; i < Equipment::getWashingMachines().size(); i++)
	{
		if (!Equipment::getWashingMachines().at(i)->isFunctional())
		{
			machineId = i;
			status = "Service worker is repairing machine " + std::to_string(machineId);
			//std::cout << "SERVICE WORKER " << __FUNCSIG__ << " 2" << std::endl;
			fixBrokenMachine();
			//std::cout << "SERVICE WORKER " << __FUNCSIG__ << " 3" << std::endl;
			return;
		}
	}
	move(1, 2);
	status = "Service worker has nothing to repair";
	return;
}

void ServiceWorker::fixBrokenMachine()
{
	//std::cout << "SERVICE WORKER " << __FUNCSIG__ << std::endl;
	int machineX = Equipment::getWashingMachines().at(machineId)->getX();
	int machineY = Equipment::getWashingMachines().at(machineId)->getY();
	move(machineX - 1, machineY);
	Equipment::getWashingMachines().at(machineId)->repairMachine();
}
