#include "WashingMachine.h"

#include <chrono>
#include <iostream>
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

void WashingMachine::setX(int x)
{
	this->x = x;
}

void WashingMachine::setY(int y)
{
	this->y = y;
}

void WashingMachine::display()
{
    //start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);

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

bool WashingMachine::useIfAvailable(int id)
{
    //std::cout << std::to_string(id) << " " << __FUNCSIG__ << " 1" << std::endl;
    std::unique_lock<std::mutex> lock(mtx);
    if (available) {
        this->available = false;
        //std::cout << std::to_string(id) << " " << __FUNCSIG__ << " 2" << std::endl;
        //std::cout << "functional = " << functional.load() << std::endl;
        cv.wait(lock, [this]() {return isFunctional(); });
        //std::cout << std::to_string(id) << " " << __FUNCSIG__ << " 3" << std::endl;
        return true;
    }
    else {
        return false;
    }
}

void WashingMachine::setAvailability(bool set, int id)
{
    //std::cout << std::to_string(id) << " " << __FUNCSIG__ << std::endl;
    std::lock_guard<std::mutex> lock(mtx);
    this->available = set;
}

void WashingMachine::repairMachine()
{
    //std::cout << __FUNCSIG__ << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    setFunctional(true);
    cv.notify_one();
}

void WashingMachine::setFunctional(bool set)
{
    //std::cout << __FUNCSIG__ << std::endl;
    //std::lock_guard<std::mutex> lock(mtx);
    this->functional = set;
}

bool WashingMachine::isFunctional()
{
    return this->functional.load();
}
