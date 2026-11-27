#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"

// Modular simulation loop using primitive variables

void runSimulation(int& playerFloor, int& playerRoom,
    int& energy, int& time, int& score,
    char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
    bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
    char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
    char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
    int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])

{


}

static int findEntity(const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
    int floor, int room, int count, char symbol)
{
    for (int i = 0; i < count; i++)
    {
        if (roomEntities[floor][room][i] == symbol)
            return i;           // slot number
    }
    return -1;                  // not in this room
}


bool handleMoveCommand(int targetFloor, int targetRoom,
    int& currentFloor, int& currentRoom,
    int& energy, int& time,
    const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
    const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
    int& emergencyTripsLeft, bool lift2Operational)
{


}


bool handleInspectCommand(int currentFloor, int currentRoom,
    int& energy, int& time,
    const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
    int entityCount,
    bool objectives[NUM_OBJECTIVES], int& score)
{

    applyResourceDelta(energy, time, COST_INSPECT_ENERGY, COST_INSPECT_TIME);
    renderRoomDetails(currentFloor, currentRoom, roomEntities, entityCount);


    if ((findEntity(roomEntities, currentFloor, currentRoom, entityCount, ENT_CLUE_2) != -1) && (!objectives[OBJ_CLUE_2]))
    {
        objectives[OBJ_CLUE_2] = true;
        score += SCORE_CLUE_FOUND;
    }

    if ((findEntity(roomEntities, currentFloor, currentRoom, entityCount, ENT_CLUE_4) != -1) && (!objectives[OBJ_CLUE_4]))
    {
        objectives[OBJ_CLUE_4] = true;
        score += SCORE_CLUE_FOUND;
    }
    
    if ((findEntity(roomEntities, currentFloor, currentRoom, entityCount, ENT_CLUE_7) != -1) && (!objectives[OBJ_CLUE_7]))
    {
        objectives[OBJ_CLUE_7] = true;
        score += SCORE_CLUE_FOUND;
    }

    return true;


}


bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,
    int& energy, int& time, int& score,
    char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
    int& entityCount,
    char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
    bool objectives[NUM_OBJECTIVES])
{

    int index = findEntity(roomEntities, currentFloor, currentRoom, entityCount, itemSymbol);

    if (index == -1)
        return false;

    if (itemSymbol == ENT_CAT || itemSymbol == ENT_PASSENGER || itemSymbol == ENT_CONTROL_PANEL)
        return false;

    if (itemSymbol == ENT_CABINET_TRAIN_KEY && !objectives[OBJ_CABINET_UNLOCKED])
        return false;

    if (!addItem(inventory, inventoryCount, itemSymbol))
        return false;

    for (int i = index; i < entityCount - 1; i++)
    {
        roomEntities[currentFloor][currentRoom][i] = roomEntities[currentFloor][currentRoom][i + 1];
    }

    roomEntities[currentFloor][currentRoom][entityCount - 1] = ENT_NONE;
    entityCount--;

    applyResourceDelta(energy, time, COST_ITEM_PICKUP_ENERGY, COST_ITEM_PICKUP_TIME);

    if (itemSymbol == ENT_MASTER_KEY)
    {
        objectives[OBJ_MASTER_KEY] = true;
        score += SCORE_KEY_COLLECT;
    }

    if (itemSymbol == ENT_FUSE)
    {
        objectives[OBJ_FUSE_COLLECTED] = true;
    }

    if (itemSymbol == ENT_LIFT_PASS)

    {
        objectives[OBJ_LIFT_PASS] = true;
        score += SCORE_LIFT_PASS;
    }

    if (itemSymbol == ENT_CABINET_TRAIN_KEY)
    {
        objectives[OBJ_TRAIN_KEY] = true;
        score += SCORE_TRAIN_KEY;
    }
    if (itemSymbol == ENT_CAT_CARRIER)
    {
        objectives[OBJ_CAT_CARRIER] = true;
    }


    return true;

}


bool handleUseCommand(char itemSymbol, int currentFloor, int currentRoom,
    int& energy, int& time, int& score,
    char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
    bool objectives[NUM_OBJECTIVES])

{

    if (!(hasItem(inventory, inventoryCount, itemSymbol)))
        return false;

    bool used = false;

    if (itemSymbol == ENT_ENERGY_PACK)
    {
        applyResourceDelta(energy, time, BONUS_ENERGY_PACK, 0);
        used = true;

    }

    if (itemSymbol == ENT_OBSERVATION_TOKEN)
    {
        applyResourceDelta(energy, time, BONUS_OBSERVATION_TOKEN, 0);
        used = true;
    }

    if (itemSymbol == ENT_FUSE)
    {
        used = installFuseControlPanel(inventory, inventoryCount, objectives, score);
    }

    if (itemSymbol == ENT_CAT_CARRIER)
    {
        if (currentRoom == 2 && currentFloor == 2)
        {
            objectives[OBJ_CAT_SECURED] = true;
            score += SCORE_SECURE_CAT;
            removeItem(inventory, inventoryCount, ENT_CAT_CARRIER);
        }

    }

    if (itemSymbol != ENT_EMERGENCY_TOOLKIT && !addItem(inventory, inventoryCount, itemSymbol))
        return false;

    if (itemSymbol == ENT_EMERGENCY_TOOLKIT)
        applyResourceDelta(energy, time, BONUS_EMERGENCY_TOOLKIT, 0);

    if(itemSymbol == ENT_MEDICAL_KIT)
    {
        if (currentFloor == 3 && currentRoom == 0 && !objectives[OBJ_PASSENGER_RESCUED])
        {
            objectives[OBJ_PASSENGER_RESCUED] = true;
            score += SCORE_RESCUE_PASSENGER;
            removeItem(inventory, inventoryCount, itemSymbol);
            used = true;
        }


    }

    return true;


}


bool handleUnlockCabinetCommand(const char* code, int currentFloor, int currentRoom,
    bool objectives[NUM_OBJECTIVES], int& score,
    char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
    int& entityCount)

{
    if (currentFloor != 3 || currentRoom != 4)
        return false;

    if (objectives[OBJ_CABINET_UNLOCKED]) 
        return false;

    if (!verifyCabinetCode(code, objectives))
        return false;

    objectives[OBJ_CABINET_UNLOCKED] = true;
    score += SCORE_CABINET_UNLOCK;
    return true;

}


bool checkEndConditions(int energy, int time, const bool objectives[NUM_OBJECTIVES])

{


}



