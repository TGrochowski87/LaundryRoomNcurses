#include "WashingMachine.h"

#include <chrono>
#include <string>
#include <thread>

WashingMachine::WashingMachine(WINDOW* win, int x, int y)
    : functional{true}
{
    this->window = win;
    this->x = x;
	this->y = y;
}

int WashingMachine::getX()
{
	return this->x;
}

int WashingMachine::getY()
{
	return this->y;
}

void WashingMachine::display()
{
    if(this->functional && this->available)
    {
        wattron(window, COLOR_PAIR(1));
        mvwaddch(window, y, x, 'M');
        wattroff(window, COLOR_PAIR(1));
    }
    else if(!this->functional)
    {
        wattron(window, COLOR_PAIR(2));
        mvwaddch(window, y, x, 'M');
        wattroff(window, COLOR_PAIR(2));
    }
    else
    {
        wattron(window, COLOR_PAIR(3));
        mvwaddch(window, y, x, 'M');
        wattroff(window, COLOR_PAIR(3));
    }
}

bool WashingMachine::useIfAvailable()
{
    std::unique_lock<std::mutex> lock(mtx);
    if (available) {
        this->available = false;
        cv.wait(lock, [this]() {return isFunctional(); });
        return true;
    }
    else {
        return false;
    }
}

void WashingMachine::setAvailability(bool set)
{
    std::lock_guard<std::mutex> lock(mtx);
    this->available = set;
}

void WashingMachine::repairMachine()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    setFunctional(true);
    cv.notify_one();
}

void WashingMachine::setFunctional(bool set)
{
    this->functional = set;
}

bool WashingMachine::isFunctional()
{
    return this->functional.load();
}
