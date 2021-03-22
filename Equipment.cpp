#include "Equipment.h"

std::vector<WashingMachine*> Equipment::washingMachines;
std::vector<DryingRack*> Equipment::dryingRacks;

std::vector<DryingRack*>& Equipment::getDryingRacks()
{
    return dryingRacks;
}

std::vector<WashingMachine*>& Equipment::getWashingMachines()
{
    return washingMachines;
}
