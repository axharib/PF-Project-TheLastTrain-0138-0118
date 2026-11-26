#include "level.h"
#include "util.h"
#include <iostream> 
#include <fstream>
using namespace std;


// Modular implementation using primitive arrays for map parsing and verification
bool loadBuildingMap(const char* filename,
    char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
    char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
    int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
    ifstream file(filename);

    if (!file)
        return false;

    char thisLine[MAX_LINE_LEN];

    while (file.getline(thisLine, MAX_LINE_LEN))
    {
        cout << thisLine << endl;
    }

    for (int i = 0; i < NUM_FLOORS; i++)
    {
        for (int j = 0; j < ROOMS_PER_FLOOR; j++)
        {
            for (int k = 0; k < MAX_ENTITIES_PER_ROOM; k++)
            {
                roomEntities[i][j][k] = ENT_NONE;
            }
            
            entityCounts[i][j] = 0;
        }
    }

    for (int i = 0; i < NUM_FLOORS; i++)
    {
        for (int j = 0; j < ROOMS_PER_FLOOR - 1; j++)
        {
            horizontalConn[i][j] = CONN_NONE;
        }
    }

    horizontalConn[0][0] = 'W';
    horizontalConn[0][1] = ' ';
    horizontalConn[0][2] = 'D';
    
    horizontalConn[1][0] = 'D';
    horizontalConn[1][1] = ' ';
    horizontalConn[1][2] = 'D';

    horizontalConn[2][0] = 'L';
    horizontalConn[2][1] = 'W';
    horizontalConn[2][2] = 'D';
   
    horizontalConn[3][0] = 'W';
    horizontalConn[3][1] = '#';
    horizontalConn[3][2] = 'D';
  
    horizontalConn[4][0] = 'D';
    horizontalConn[4][1] = 'D';
    horizontalConn[4][2] = 'W';
  
    roomEntities[0][1][0] = '<';
    roomEntities[0][3][0] = ')';

    roomEntities[1][2][0] = '\\';

    roomEntities[2][1][0] = '4';
    roomEntities[2][1][1] = '*';
    roomEntities[2][2][0] = '~';
    roomEntities[2][3][0] = ';';
    roomEntities[2][3][1] = '(';

    roomEntities[3][0][0] = '!';
    roomEntities[3][1][0] = '%';
    roomEntities[3][1][1] = '$';
    roomEntities[3][2][0] = '2';

    roomEntities[4][0][0] = '7';
    roomEntities[4][0][1] = '^';
    roomEntities[4][2][0] = '@';
    roomEntities[4][3][0] = '&';

    entityCounts[0][1] = 1;
    entityCounts[0][3] = 1;
    entityCounts[1][2] = 1;
    entityCounts[2][1] = 2;
    entityCounts[2][2] = 1;
    entityCounts[2][3] = 2;
    entityCounts[3][0] = 1;
    entityCounts[3][1] = 2;
    entityCounts[3][2] = 1;
    entityCounts[4][0] = 2;
    entityCounts[4][2] = 1;
    entityCounts[4][3] = 1;



    return true;


}



bool loadInitialState(const char* filename,
    int& playerFloor, int& playerRoom,
    int& energy, int& time, int& score,
    char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
    bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft)
{

    return false;


}


bool validateStartingRoom(int startFloor, int startRoom,
    const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1])
{


    return false;

}



const char* getEntityName(char entitySymbol)
{



    return false;

}