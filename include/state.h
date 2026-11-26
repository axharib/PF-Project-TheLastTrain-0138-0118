#ifndef STATE_H
#define STATE_H

#include "config.h"

// Connection Type Constants (Stored in char or int 2D matrices)
const char CONN_NONE        = ' ';
const char CONN_DOOR        = 'D'; // Ordinary door
const char CONN_WINDOW      = 'W'; // Window traversal
const char CONN_LOCKED_DOOR = 'L'; // Locked door (requires Master Key)
const char CONN_SOLID_WALL  = '#'; // Wall partition (impassable)

// self added:
const char CONN_STAIRCASE   = 'S'; // Staircase
const char CONN_LIFT1       = '1'; // Lift 1
const char CONN_LIFT2       = '2'; // Lift 2

// Entity / Item Character Symbols (Stored in 2D/3D char arrays)
const char ENT_NONE             = ' ';
const char ENT_MASTER_KEY       = '<'; // Master Key
const char ENT_ENERGY_PACK      = ')'; // Energy Pack
const char ENT_EMERGENCY_TOOLKIT= '\\';// Emergency Toolkit
const char ENT_CLUE_4           = '4'; // Clue Digit 4
const char ENT_FUSE             = '*'; // Fuse
const char ENT_CAT              = '~'; // Stranded Cat
const char ENT_CONTROL_PANEL    = ';'; // Control Panel Terminal
const char ENT_LIFT_PASS        = '('; // Lift Pass
const char ENT_PASSENGER        = '!'; // Stranded Passenger
const char ENT_MEDICAL_KIT      = '%'; // Medical Kit
const char ENT_EMERGENCY_CARD   = '$'; // Emergency Access Card
const char ENT_CLUE_2           = '2'; // Clue Digit 2
const char ENT_CLUE_7           = '7'; // Clue Digit 7
const char ENT_CABINET_TRAIN_KEY= '^'; // Cabinet / Train Key
const char ENT_CAT_CARRIER      = '@'; // Cat Carrier
const char ENT_OBSERVATION_TOKEN= '&'; // Observation Token

// Index Constants for Parallel Objective Booleans
const int OBJ_MASTER_KEY        = 0;
const int OBJ_CLUE_4            = 1;
const int OBJ_CLUE_2            = 2;
const int OBJ_CLUE_7            = 3;
const int OBJ_CABINET_UNLOCKED  = 4;
const int OBJ_TRAIN_KEY         = 5;
const int OBJ_FUSE_COLLECTED    = 6;
const int OBJ_FUSE_INSTALLED    = 7;
const int OBJ_LIFT_PASS         = 8;
const int OBJ_LIFT2_ACTIVE      = 9;
const int OBJ_PASSENGER_RESCUED = 10;
const int OBJ_CAT_CARRIER       = 11;
const int OBJ_CAT_SECURED       = 12;
const int OBJ_TRAIN_PREPARED    = 13;
const int OBJ_MISSION_WON       = 14;
const int OBJ_MISSION_FAILED    = 15;
const int NUM_OBJECTIVES        = 16;

#endif // STATE_H
