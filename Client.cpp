#include "Client.h"

#include <chrono>
#include <pthread.h>

#include "Equipment.h"

Client::Client(WINDOW* win, int laundryTime, int dryingTime, int id, int x, int y)
	: isDoingLaundry{true}
	, laundryStatus{LaundryStatus::dirty}
	, initX{x}
	, initY{y}
{
	this->laundryTime = laundryTime + (rand() % 41 - 20) * laundryTime / 100;
	this->dryingTime = dryingTime + (rand() % 41 - 20) * dryingTime / 100;

	this->window = win;
	this->x = x;
	this->y = y;
	this->id = id;
}

int Client::getId()
{
	return id;
}

void Client::display()
{
	mvwaddch(window, y, x, 'C');
}

void Client::move(int newX, int newY)
{
	mvwaddch(window, y, x, ' ');
	x = newX;
	y = newY;
}

float Client::getWaitingTime()
{
	return waitingTime;
}

int Client::getLaundryCounter()
{
	return laundryCounter;
}

void Client::start()
{
	lifeThread = std::make_unique<std::thread>([this]() {act(); });
}

void Client::stop()
{
	this->isDoingLaundry = false;
	this->lifeThread->join();
}

void Client::act()
{
	while (this->isDoingLaundry.load())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		if (this->laundryStatus == LaundryStatus::dirty)
		{
			if (searchForFreeMachine())
			{
				this->status = "Client " + std::to_string(id) + " is using washing mashine " + std::to_string(occupiedId);
				useWashingMachine();
			}
			else
			{
				this->status = "Client " + std::to_string(id) + " is waiting for washing machine";
				this->waitingTime += 0.1;
			}
		}
		else if (this->laundryStatus == LaundryStatus::wet)
		{
			if (searchForFreeRack())
			{
				this->status = "Client " + std::to_string(id) + " is using drying rack " + std::to_string(occupiedId);
				useDryingRack();
			}
			else
			{
				this->status = "Client " + std::to_string(id) + " is waiting for drying rack";
				this->waitingTime += 0.1;
			}
		}
		else
		{
			laundryCounter++;
			this->status = "Client " + std::to_string(id) + " is getting their clothes dirty again";
			getClothesDirty();
		}
	}
}

std::string Client::getStatus()
{
	return status;
}

void Client::useWashingMachine()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(laundryTime));

	if (rand() % 101 > 80)
	{
		Equipment::getWashingMachines().at(occupiedId)->setFunctional(false);
	}

	laundryStatus = LaundryStatus::wet;
	freeMachine();
	status = "Client " + std::to_string(id) + " has freed washing machine " + std::to_string(occupiedId);
}

void Client::useDryingRack()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(dryingTime));
	laundryStatus = LaundryStatus::clean;
	freeRack();
	status = "Client " + std::to_string(id) + " has freed drying rack " + std::to_string(occupiedId);
	
}

void Client::getClothesDirty()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	laundryStatus = LaundryStatus::dirty;
	this->status = "Client " + std::to_string(id) + " goes to laundry room";
}

void Client::freeMachine()
{
	Equipment::getWashingMachines().at(occupiedId)->setAvailability(true);
	move(initX, initY);
	occupiedId = 0;
}

void Client::freeRack()
{
	Equipment::getDryingRacks().at(occupiedId)->setAvailability(true);
	move(initX, initY);
	occupiedId = 0;
}

bool Client::searchForFreeMachine()
{
	for (int i = 0; i < Equipment::getWashingMachines().size(); i++)
	{
		if (Equipment::getWashingMachines().at(i)->useIfAvailable())
		{
			occupiedId = i;

			int machineX = Equipment::getWashingMachines().at(occupiedId)->getX();
			int machineY = Equipment::getWashingMachines().at(occupiedId)->getY();
			move(machineX + 1, machineY);
			return true;
		}
	}
	return false;
}

bool Client::searchForFreeRack()
{
	for (int i = 0; i < Equipment::getDryingRacks().size(); i++)
	{
		if (Equipment::getDryingRacks().at(i)->useIfAvailable())
		{
			occupiedId = i;

			int rackX = Equipment::getDryingRacks().at(occupiedId)->getX();
			int rackY = Equipment::getDryingRacks().at(occupiedId)->getY();
			move(rackX - 1, rackY);
			return true;
		}
	}
	return false;
}
