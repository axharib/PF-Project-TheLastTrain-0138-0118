#include "switches.h"
#include "util.h"

// Door lock mechanics, lifts, and cabinet verification without structures

bool canUseLift1(int fromFloor, int toFloor)
{

	if (fromFloor == 1 && toFloor == 2)
		return true;
	if (fromFloor == 2 && toFloor == 1)
		return true;

	return false;

}

bool canUseLift2(int fromFloor, int toFloor, bool lift2Operational, int emergencyTripsLeft)
{
    bool validRoute =
        (fromFloor == 2 && (toFloor == 4 || toFloor == 5)) ||
        (fromFloor == 4 && (toFloor == 2 || toFloor == 5)) ||
        (fromFloor == 5 && (toFloor == 2 || toFloor == 4));

    if (!validRoute)
        return false;

    if (lift2Operational)
        return true;

    if (emergencyTripsLeft > 0)
        return true;

    return false;
}

bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount)
{
    for (int i = 0; i < inventoryCount; i++)
    {
        if (inventory[i] == '<')
            return true;
    }

    return false;

}
bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, bool objectives[NUM_OBJECTIVES], int& score)
{

    

    if (!objectives[OBJ_FUSE_COLLECTED] || objectives[OBJ_FUSE_INSTALLED])
        return false;

    int index = -1;

    for (int i = 0; i < inventoryCount; i++)
    {

        if (inventory[i] == '*')
        {

            index = i;
            break;
        }
    }

    if (index == -1)
        return false;

    for (int i = index; i < inventoryCount - 1 ; i++)
    {
        inventory[i] = inventory[i + 1];

    }

    inventoryCount--;

    objectives[OBJ_FUSE_INSTALLED] = 1;
    objectives[OBJ_LIFT2_ACTIVE] = 1;
    score += 20;
    return true;
}

bool verifyCabinetCode(const char* enteredCode, const bool objectives[NUM_OBJECTIVES])
{

    if (objectives[OBJ_CLUE_4] && objectives[OBJ_CLUE_2] && objectives[OBJ_CLUE_7])
    {
        if (stringEquals(enteredCode, "427"))
            return true;

    }

    return false;
}