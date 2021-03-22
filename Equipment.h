#pragma once
#include <vector>

#include "DryingRack.h"
#include "WashingMachine.h"

class Equipment
{
public:
	static std::vector<DryingRack*>& getDryingRacks();
	static std::vector<WashingMachine*>& getWashingMachines();

private:
	static std::vector<WashingMachine*> washingMachines;
	static std::vector<DryingRack*> dryingRacks;
};

