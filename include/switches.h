#ifndef SWITCHES_H
#define SWITCHES_H

#include "state.h"

// Access control & puzzle validation without structs
bool canUseLift1(int fromFloor, int toFloor);
bool canUseLift2(int fromFloor, int fromRoom, int toRoom, int toFloor, bool lift2Operational, int emergencyTripsLeft);

bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount);
bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                             bool objectives[NUM_OBJECTIVES], int& score);
bool verifyCabinetCode(const char* enteredCode, const bool objectives[NUM_OBJECTIVES]);

#endif // SWITCHES_H
