#include "Room.h"

#include <iostream>

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
		Equipment::getDryingRacks().push_back(new DryingRack(47, 2 + i*2));
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
		// initscr();
		// noecho();

		int machnieCount = Equipment::getWashingMachines().size();
		int rackCount = Equipment::getDryingRacks().size();
		int clientCount = this->clients.size();

		int x = ((machnieCount > rackCount) ? machnieCount : rackCount) > clientCount ? (machnieCount > rackCount) ? machnieCount : rackCount : clientCount;
		x = x*2 + 3;
		int y = 50;

		//WINDOW* win = newwin(x, y, 1, 1);
		refresh();

		box(this->win, 0, 0);

		// for(int i = 0; i < machnieCount; i++)
		// {
		// 	mvwprintw(this->win, 2 + i*2, 4, "M");
		// }
		for(int i = 0; i < rackCount; i++)
		{
			mvwprintw(this->win, 2 + i*2, 47, "R");
		}
		//mvwprintw(win, 1, 2, "S");

		for(int i = 0; i < clientCount; i++)
		{
			mvwprintw(this->win, 2 + i*2, 24, "C");
		}

		// init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    	// wattron(win, COLOR_PAIR(1));
    	// mvwaddch(win, 10, 25, 'M');
    	// wattroff(win, COLOR_PAIR(1));

		wrefresh(this->win);

		//mvwprintw(win, 10, 20, "S");
		//refresh();
		//wrefresh(win);

		while (roomIsOpen.load())
		{
			this->serviceWorker->display();
			for(WashingMachine* m : Equipment::getWashingMachines())
			{
				m->display();
			}

			
			wrefresh(this->win);

			// for (Client* c : clients)
			// {
			// 	std::cout << c->getStatus() << std::endl;
			// }
			// std::cout << std::endl;
			// std::cout << serviceWorker->getStatus() << std::endl;
			// std::cout << "\n------------------------------------------" << std::endl;

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		});
}

void Room::endSimulation()
{
	for (Client* c : clients)
	{
		//std::cout << "YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY " << c->getId() << std::endl;
		c->stop();
		//std::cout << "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ" << std::endl;
	}
	serviceWorker->stop();
	roomIsOpen = false;
	showStatusThread->join();

	for (Client* c : clients)
	{
		std::cout << std::endl;
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "Client " << c->getId() << ":" << std::endl;
		std::cout << "Has done laundry " << c->getLaundryCounter() << " times." << std::endl;
		std::cout << "Was waiting for " << c->getWaitingTime() << " seconds." << std::endl;
		delete c;
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
