#include "Room.h"

#include <string>

#include "Equipment.h"
#include "WashingMachine.h"
#include "DryingRack.h"

Room::Room()
	: roomIsOpen{true}
{
}

void Room::initializeRoom(int amountOfClients, int amountOfMachines, int amountOfRacks, int laundryTime, int dryingTime)
{
	int x = ((amountOfMachines > amountOfRacks) ? amountOfMachines : amountOfRacks) > amountOfClients ?
	 (amountOfMachines > amountOfRacks) ? amountOfMachines : amountOfRacks : amountOfClients;
	x = x*2 + 3;
	int y = 50;
	win = newwin(x, y, 1, 1);
	start_color();


	for (int i = 0; i < amountOfClients; i++)
	{
		this->clients.push_back(new Client(win, laundryTime, dryingTime, i, 24, 2 + i*2));
	}
	for (int i = 0; i < amountOfRacks; i++)
	{
		Equipment::getDryingRacks().push_back(new DryingRack(win, 47, 2 + i*2));
	}
	for (int i = 0; i < amountOfMachines; i++)
	{
		Equipment::getWashingMachines().push_back(new WashingMachine(win, 4, 2 + i*2));
	}
	this->serviceWorker = new ServiceWorker(win, 2, 1);
}

void Room::runSimulation()
{
	for (Client* c : clients)
	{
		c->start();
	}
	serviceWorker->start();

	bool isEveryoneReady = false;
	while (!isEveryoneReady)
	{
		for (Client* c : clients)
		{
			if (!c->getStatus().empty())
			{
				isEveryoneReady = true;
			}
			else
			{
				isEveryoneReady = false;
				break;
			}
		}
	}

	showStatusThread = std::make_unique<std::thread>([this]() {
		refresh();

		box(this->win, 0, 0);

		wrefresh(this->win);

		init_pair(1, COLOR_GREEN, COLOR_BLACK);
    	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    	init_pair(3, COLOR_RED, COLOR_BLACK);

		while (roomIsOpen.load())
		{
			this->serviceWorker->display();
			for(WashingMachine* m : Equipment::getWashingMachines())
			{
				m->display();
			}
			for(DryingRack* r : Equipment::getDryingRacks())
			{
				r->display();
			}
			for(Client* c : this->clients)
			{
				c->display();
			}

			
			wrefresh(this->win);

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		});
}

void Room::endSimulation()
{
	for (Client* c : clients)
	{
		c->stop();
	}
	serviceWorker->stop();
	roomIsOpen = false;
	showStatusThread->join();

	wclear(win);
	endwin();

	for(int i = 0; i < clients.size(); i++)
	{
		std::string notification = "Client " + std::to_string(clients.at(i)->getId()) + " has done their laundry " + std::to_string(clients.at(i)->getLaundryCounter()) + " times.";
		printf("%s\n", notification.c_str());
		delete clients.at(i);
	}
	
	delete serviceWorker;

	for (WashingMachine* m : Equipment::getWashingMachines())
	{
		delete m;
	}
	for (DryingRack* r : Equipment::getDryingRacks())
	{
		delete r;
	}
}
