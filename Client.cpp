#include "Client.h"

#include <chrono>
#include <iostream>
#include <pthread.h>

#include "Equipment.h"

Client::Client(WINDOW* win, int laundryTime, int dryingTime, int id, int x, int y)
	: isDoingLaundry{true}
	, laundryStatus{LaundryStatus::dirty}
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

int Client::getX()
{
	return this->x;
}

int Client::getY()
{
	return this->y;
}

void Client::setX(int x)
{
	this->x = x;
}

void Client::setY(int y)
{
	this->y = y;
}

void Client::display()
{
	mvwaddch(window, y, x, 'S');
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
		//std::cout << std::to_string(id) << " " << __FUNCSIG__ << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		if (this->laundryStatus == LaundryStatus::dirty)
		{
			if (searchForFreeMachine())
			{
				//std::cout << "11111111111111111111111111111111111111111111111111111111111" << std::endl;
				this->status = "Client " + std::to_string(id) + " is using washing mashine " + std::to_string(occupiedId);
				useWashingMachine();
				//std::cout << "22222222222222222222222222222222222222222222222222222222222" << std::endl;
			}
			else
			{
				this->status = "Client " + std::to_string(id) + " is waiting for washing machine";
				this->waitingTime += 0.1;
			}
			//std::cout << "STATUS = " << this->isDoingLaundry.load() << std::endl;
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
			//std::cout << "33333333333333333333333333333333333333333333333333333333333333" << std::endl;
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
	//std::cout << std::to_string(id) << " " << __FUNCSIG__ << std::endl;
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
	//std::cout << std::to_string(id) << " " << __FUNCSIG__ << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(dryingTime));
	laundryStatus = LaundryStatus::clean;
	freeRack();
	status = "Client " + std::to_string(id) + " has freed drying rack " + std::to_string(occupiedId);
	
}

void Client::getClothesDirty()
{
	//std::cout << std::to_string(id) << " " << __FUNCSIG__ << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	laundryStatus = LaundryStatus::dirty;
	this->status = "Client " + std::to_string(id) + " goes to laundry room";
	//std::cout << "SDGDRSHGDSFHGDZ" << std::endl;
}

void Client::freeMachine()
{
	//std::cout << std::to_string(id) << " " << __FUNCSIG__ << std::endl;
	Equipment::getWashingMachines().at(occupiedId)->setAvailability(true, id);
	occupiedId = 0;
}

void Client::freeRack()
{
	//std::cout << std::to_string(id) << " " << __FUNCSIG__ << std::endl;
	Equipment::getDryingRacks().at(occupiedId)->setAvailability(true);
	occupiedId = 0;
}

bool Client::searchForFreeMachine()
{
	//std::cout << std::to_string(id) << " " << __FUNCSIG__ << std::endl;
	for (int i = 0; i < Equipment::getWashingMachines().size(); i++)
	{
		if (Equipment::getWashingMachines().at(i)->useIfAvailable(id))
		{
			occupiedId = i;
			return true;
		}
	}
	return false;
}

bool Client::searchForFreeRack()
{
	//std::cout << std::to_string(id) << " " << __FUNCSIG__ << std::endl;
	for (int i = 0; i < Equipment::getDryingRacks().size(); i++)
	{
		if (Equipment::getDryingRacks().at(i)->useIfAvailable())
		{
			occupiedId = i;
			return true;
		}
	}
	return false;
}
