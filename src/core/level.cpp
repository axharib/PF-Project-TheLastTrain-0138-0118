#include "level.h"
#include "util.h"

// Modular implementation using primitive arrays for map parsing and verification
#include "level.h"
#include "util.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

// ---------------- ENTITY NAMES ----------------
// turns a symbol like '<' into "Master Key"
const char* getEntityName(char entity_symbol)
{
    switch (entity_symbol) {
    case ENT_MASTER_KEY:
        return "Master Key";
    case ENT_ENERGY_PACK:
        return "Energy Pack";
    case ENT_EMERGENCY_TOOLKIT:
        return "Emergency Toolkit";
    case ENT_CLUE_4:
        return "Clue Digit 4";
    case ENT_FUSE:
        return "Fuse";
    case ENT_CAT:
        return "Cat";
    case ENT_CONTROL_PANEL:
        return "Control Panel";
    case ENT_LIFT_PASS:
        return "Lift Pass";
    case ENT_PASSENGER:
        return "Injured Passenger";
    case ENT_MEDICAL_KIT:
        return "Medical Kit";
    case ENT_EMERGENCY_CARD:
        return "Emergency Access Card";
    case ENT_CLUE_2:
        return "Clue Digit 2";
    case ENT_CLUE_7:
        return "Clue Digit 7";
    case ENT_CABINET_TRAIN_KEY:
        return "Cabinet / Train Key";
    case ENT_CAT_CARRIER:
        return "Cat Carrier";
    case ENT_OBSERVATION_TOKEN:
        return "Observation Token";
    default:
        return "Unknown";
    }
}

// ---------------- START ROOM CHECK ----------------
// is the chosen start room a real room, and is the map loaded?
bool validateStartingRoom(int start_floor, int start_room,
    const char horizontal_conn[NUM_FLOORS][ROOMS_PER_FLOOR - 1])
{

    if (start_floor < 1 || start_floor > NUM_FLOORS) {
        cout << "That floor doesn't exist. Pick 1 to " << NUM_FLOORS << ".\n";
        return false;
    }


    if (start_room < 1 || start_room > ROOMS_PER_FLOOR) {
        cout << "That room doesn't exist. Pick 1 to " << ROOMS_PER_FLOOR << ".\n";
        return false;
    }


    bool map_has_links = false;
    for (int floor_index = 0; floor_index < NUM_FLOORS; floor_index++) {
        for (int gap_index = 0; gap_index < ROOMS_PER_FLOOR - 1; gap_index++) {
            if (horizontal_conn[floor_index][gap_index] != CONN_NONE) {
                map_has_links = true;
            }
        }
    }
    if (!map_has_links) {
        cout << "The building map isn't loaded.\n";
        return false;
    }

    return true;
}

// ---------------- INITIAL STATE ----------------
// reads initial-state.txt word by word (like "INITIAL_ENERGY: 100")
bool loadInitialState(const char* file_name,
    int& player_floor, int& player_room,
    int& energy, int& time, int& score,
    char inventory[MAX_INVENTORY_CAPACITY], int& inventory_count,
    bool objectives[NUM_OBJECTIVES], int& emergency_trips_left)
{

    fstream state_file(file_name, ios::in);
    if (!state_file.is_open()) {
        cout << "Error: could not open " << file_name << "\n";
        return false;
    }


    energy = INITIAL_ENERGY;
    time = INITIAL_TIME;
    score = INITIAL_SCORE;
    inventory_count = 0;
    for (int i = 0; i < MAX_INVENTORY_CAPACITY; i++) {
        inventory[i] = ENT_NONE;
    }
    for (int i = 0; i < NUM_OBJECTIVES; i++) {
        objectives[i] = false;
    }
    emergency_trips_left = 0;


    player_floor = 0;
    player_room = 0;

    char setting_name[MAX_LINE_LEN];


    while (state_file >> setting_name) {

        if (stringEquals(setting_name, "INITIAL_ENERGY:")) {
            int number;
            state_file >> number;   // >> turns "100" into 100 for us
            if (!state_file || number <= 0 || number > MAX_ENERGY) {
                cout << "Bad INITIAL_ENERGY in " << file_name << "\n";
                return false;
            }
            energy = number;
        }
        else if (stringEquals(setting_name, "INITIAL_TIME:")) {
            int number;
            state_file >> number;
            if (!state_file || number <= 0 || number > MAX_TIME) {
                cout << "Bad INITIAL_TIME in " << file_name << "\n";
                return false;
            }
            time = number;
        }
        else if (stringEquals(setting_name, "INITIAL_SCORE:")) {
            int number;
            state_file >> number;
            if (!state_file || number < 0) {
                cout << "Bad INITIAL_SCORE in " << file_name << "\n";
                return false;
            }
            score = number;
        }
        else if (stringEquals(setting_name, "START_ROOM:")) {

            char room_text[MAX_LINE_LEN];
            state_file >> room_text;
            if (room_text[0] == 'F' && room_text[2] == '-' &&
                room_text[3] == 'R' && room_text[5] == '\0') {
                player_floor = room_text[1] - '0';
                player_room = room_text[4] - '0';
            }
        }
        else {

            char skip_value[MAX_LINE_LEN];
            state_file >> skip_value;
        }
    }

    state_file.close();
    return true;
}


bool loadBuildingMap(const char* file_name,
    char horizontal_conn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
    char room_entities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
    int entity_counts[NUM_FLOORS][ROOMS_PER_FLOOR])
{

    // open the file for reading
    fstream map_file(file_name, ios::in);
if (!map_file.is_open()) {
    cout << "Error: could not open " << file_name << "\n";
    return false;
}


for (int floor_index = 0; floor_index < NUM_FLOORS; floor_index++) {
    for (int gap_index = 0; gap_index < ROOMS_PER_FLOOR - 1; gap_index++) {
        horizontal_conn[floor_index][gap_index] = CONN_NONE;
    }
    for (int room_index = 0; room_index < ROOMS_PER_FLOOR; room_index++) {
        entity_counts[floor_index][room_index] = 0;
        for (int i = 0; i < MAX_ENTITIES_PER_ROOM; i++) {
            room_entities[floor_index][room_index][i] = ENT_NONE;
        }
    }
}


bool floor_found[NUM_FLOORS] = { false };

char name_line[MAX_LINE_LEN];
char items_line[MAX_LINE_LEN];


while (map_file.getline(name_line, MAX_LINE_LEN)) {


    int floor_number = 0;
    int room_spot[ROOMS_PER_FLOOR] = { -1, -1, -1, -1 };

    for (int i = 0; name_line[i] != '\0'; i++) {
        if (name_line[i] == 'F' &&
            name_line[i + 1] >= '1' && name_line[i + 1] <= '5' &&
            name_line[i + 2] == '-' &&
            name_line[i + 3] == 'R' &&
            name_line[i + 4] >= '1' && name_line[i + 4] <= '4') {

            floor_number = name_line[i + 1] - '0';
            int room_number = name_line[i + 4] - '0';
            room_spot[room_number - 1] = i;
        }
    }


    if (floor_number == 0) {
        continue;
    }


    bool all_rooms_found = true;
    for (int room_index = 0; room_index < ROOMS_PER_FLOOR; room_index++) {
        if (room_spot[room_index] == -1) {
            all_rooms_found = false;
        }
    }
    if (!all_rooms_found) {
        continue;
    }

    int floor_index = floor_number - 1;

    // links: the letter sits 2 spots before rooms 2, 3 and 4
    for (int gap_index = 0; gap_index < ROOMS_PER_FLOOR - 1; gap_index++) {
        char link = name_line[room_spot[gap_index + 1] - 2];

        if (link == CONN_DOOR || link == CONN_WINDOW ||
            link == CONN_LOCKED_DOOR || link == CONN_SOLID_WALL) {
            horizontal_conn[floor_index][gap_index] = link;
        }
        else {
            horizontal_conn[floor_index][gap_index] = CONN_NONE;   // like the "1" in L1
        }
    }

    // room 4 ends at the next '|' on the name line
    int last_room_end = room_spot[ROOMS_PER_FLOOR - 1];
    while (name_line[last_room_end] != '\0' && name_line[last_room_end] != '|') {
        last_room_end++;
    }

    // the items are on the very next line
    if (!map_file.getline(items_line, MAX_LINE_LEN)) {
        cout << "Error: items line missing for floor " << floor_number << "\n";
        return false;
    }

    // how long is the items line?
    int items_length = 0;
    while (items_line[items_length] != '\0') {
        items_length++;
    }

    // look inside each room's columns for items
    for (int room_index = 0; room_index < ROOMS_PER_FLOOR; room_index++) {

        int start_column = room_spot[room_index];
        int end_column;
        if (room_index < ROOMS_PER_FLOOR - 1) {
            end_column = room_spot[room_index + 1] - 2;   // stop at the link
        }
        else {
            end_column = last_room_end;                   // stop at the outer wall
        }

        for (int column = start_column; column < end_column && column < items_length; column++) {
            char thing = items_line[column];

            // anything that isn't a space or wall is an item
            if (thing != ' ' && thing != '|' && thing != '\r') {
                int count = entity_counts[floor_index][room_index];
                if (count < MAX_ENTITIES_PER_ROOM) {
                    room_entities[floor_index][room_index][count] = thing;
                    entity_counts[floor_index][room_index]++;
                }
            }
        }
    }

    floor_found[floor_index] = true;
}

map_file.close();


for (int floor_index = 0; floor_index < NUM_FLOORS; floor_index++) {
    if (!floor_found[floor_index]) {
        cout << "Error: floor " << floor_index + 1 << " is missing from " << file_name << "\n";
        return false;
    }
}

return true;
}