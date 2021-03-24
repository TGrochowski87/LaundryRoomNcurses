#pragma once
#include <atomic>
#include <vector>
#include <thread>
#include <ncurses.h>

#include "Client.h"
#include "ServiceWorker.h"

class Room
{
public:
	Room();
	void initializeRoom(int, int, int, int, int);
	void runSimulation();
	void endSimulation();

private:
	WINDOW* win;
	std::atomic_bool roomIsOpen;
	std::unique_ptr<std::thread> showStatusThread;

	std::vector<Client*> clients;
	ServiceWorker* serviceWorker;
};

