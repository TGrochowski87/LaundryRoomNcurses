#include <iostream>
#include <ctime>
#include <ncurses.h>

#include "Room.h"

using namespace std;

int main()
{
	std::srand(time(NULL));

	Room room;

	//cout << "Laundry Room Simulation" << endl;
	printf("Laundry Room Simulation\n\r");

	//cout << "How many clients will do their laundry today?" << endl;
	printf("How many clients will do their laundry today?\n\r");
	int amountOfClients;
	//cin >> amountOfClients;
	scanf("%d", &amountOfClients);

	//cout << "How many washing machines are in the room?" << endl;
	printf("How many washing machines are in the room?\n\r");
	int amountOfMachines;
	//cin >> amountOfMachines;
	scanf("%d", &amountOfMachines);

	//cout << "How many drying racks are in the room?" << endl;
	printf("How many drying racks are in the room?\n\r");
	int amountOfRacks;
	//cin >> amountOfRacks;
	scanf("%d", &amountOfRacks);

	//cout << "How long will doing laundry take?" << endl;
	printf("How long will doing laundry take?\n\r");
	int laundryTime;
	//cin >> laundryTime;
	scanf("%d", &laundryTime);

	//cout << "How long will drying take?" << endl;
	printf("How long will drying take?\n\r");
	int dryingTime;
	//cin >> dryingTime;
	scanf("%d", &dryingTime);

	printf("\n");

	initscr();
	noecho();

	room.initializeRoom(amountOfClients, amountOfMachines, amountOfRacks, laundryTime, dryingTime);
	room.runSimulation();

	//char c = getch();
	//cout << c;
	
	while (getch() != 32) {
		//int c = getch();
		//printw("%d\n", getch());
	} 
	//cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;

	room.endSimulation();
	endwin();
	return 0;
}