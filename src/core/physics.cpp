#include "physics.h"
#include "util.h"
#include "switches.h"

// Movement validation and resource adjustments without structures



bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
    const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
    char& outConnType)
{

    

    outConnType = CONN_NONE;

    if (fromF != toF)
    {
        //lift1:

        if ((fromF == 0 && toF == 1) || (fromF == 1 && toF == 0))
        {
            if ((fromR == toR) && (fromR == 1 || fromR == 2))
            {
                outConnType = CONN_LIFT1;
                return true;
            }
        }

        //lift2:

        if ((fromF == 1) && (toF == 3 || toF == 4)
            || (fromF == 3) && (toF == 4 || toF == 1)
            || (fromF == 4) && (toF == 3 || toF == 1))
        {
            if ((fromR == toR) && fromR == 3)
            {
                outConnType = CONN_LIFT2;
                return true;
            }

        }
            



        if (fromR != toR)
            return false;

        if (fromR == 0)
        {
            if ((fromF == 2 && toF == 3) || (fromF == 3 && toF == 2))
                return false;
            if ((fromF == toF + 1) || (fromF == toF - 1))
            {
                outConnType = CONN_STAIRCASE;
                return true;
            }            
        }

        return false;
    }

    if (fromR == toR)
        return false;

    if ((fromR == toR + 1) || (fromR == toR - 1))
    {
        

        int minimum;
        if (fromR < toR)
            minimum = fromR;
        else
            minimum = toR;
        
        char conn;
        conn = horizontalConn[fromF][minimum];

        if (conn == CONN_SOLID_WALL)
            return false;
        if (conn == CONN_NONE)
            return false;

        outConnType = conn;

        return true;

        
    }

    return false;

}

bool canTraverse(int fromFloor, int fromRoom, int targetFloor, int targetRoom, char connType,
    const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
    int emergencyTripsLeft, bool lift2Operational) 
{

    if (connType == CONN_DOOR || connType == CONN_WINDOW ||
        connType == CONN_STAIRCASE || connType == CONN_LIFT1)
        return true;

   
    if (connType == CONN_SOLID_WALL)
        return false;
    if (connType == CONN_NONE)
        return false;
   


    if (connType == CONN_LOCKED_DOOR)
        return (hasItem(inventory, inventoryCount, ENT_MASTER_KEY));

    
    if (connType == CONN_LIFT2)
        return lift2Operational || emergencyTripsLeft > 0;


    return false;


}

void applyResourceDelta(int& energy, int& time, int deltaEnergy, int deltaTime)

{
    
    energy += deltaEnergy;

    if (energy > MAX_ENERGY)
        energy = MAX_ENERGY;

    time += deltaTime;

    if (time > MAX_TIME)
        time = MAX_TIME;

    if (energy < 0)
        energy = 0;

    if (time < 0)
        time = 0;


}

void applyScoreDelta(int& score, int deltaScore)
{
    score += deltaScore;
}

bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, char itemSymbol)
{

    for (int i = 0; i < inventoryCount; i++)
    {
        if (inventory[i] == itemSymbol)
        return true;
    }

    return false;

}

bool addItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol)
{

    if (inventoryCount < MAX_INVENTORY_CAPACITY)
    {
        inventory[inventoryCount] = itemSymbol;
        inventoryCount++;
        return true;
    }
    return false;

}

bool removeItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol)
{

    if (inventoryCount == 0)
        return false;

    int index = -1;

    for (int i = 0; i < inventoryCount; i++)
    {
        if (inventory[i] == itemSymbol)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
        return false;

    for (int i = index; i < inventoryCount - 1; i++)
    {
        inventory[i] = inventory[i + 1];

    }

    inventoryCount--;
    return true;

}


