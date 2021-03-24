#include "DryingRack.h"

DryingRack::DryingRack(WINDOW* win, int x, int y)
{
	this->window = win;
	this->x = x;
	this->y = y;
}

int DryingRack::getX()
{
	return this->x;
}

int DryingRack::getY()
{
	return this->y;
}

void DryingRack::display()
{
    if(this->available)
    {
        wattron(window, COLOR_PAIR(1));
        mvwaddch(window, y, x, 'R');
        wattroff(window, COLOR_PAIR(1));
    }
    else
    {
        wattron(window, COLOR_PAIR(3));
        mvwaddch(window, y, x, 'R');
        wattroff(window, COLOR_PAIR(3));
    }
}

bool DryingRack::useIfAvailable()
{
	std::lock_guard<std::mutex> lock(mtx);
	if (this->available) {
		this->available = false;
		return true;
	}
	else {
		return false;
	}
}

void DryingRack::setAvailability(bool set)
{
	std::lock_guard<std::mutex> lock(mtx);
	this->available = set;
}
