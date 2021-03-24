#include <iostream>
#include <ctime>
#include <ncurses.h>

#include "Room.h"

using namespace std;

int main()
{
	std::srand(time(NULL));

	Room room;

	printf("Laundry Room Simulation\n\r");

	printf("How many clients will do their laundry today?\n\r");
	int amountOfClients;
	scanf("%d", &amountOfClients);

	printf("How many washing machines are in the room?\n\r");
	int amountOfMachines;
	scanf("%d", &amountOfMachines);

	printf("How many drying racks are in the room?\n\r");
	int amountOfRacks;
	scanf("%d", &amountOfRacks);

	printf("How long will doing laundry take?\n\r");
	int laundryTime;
	scanf("%d", &laundryTime);

	printf("How long will drying take?\n\r");
	int dryingTime;
	scanf("%d", &dryingTime);

	printf("\n");

	initscr();
	noecho();

	room.initializeRoom(amountOfClients, amountOfMachines, amountOfRacks, laundryTime, dryingTime);
	room.runSimulation();
	
	while (getch() != 32) {} 

	room.endSimulation();
	return 0;
}