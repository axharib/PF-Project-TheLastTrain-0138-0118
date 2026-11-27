#include "viewer_iface.h"
#include "level.h"
#include "util.h"
#include <iostream>
using namespace std;

// room names, [floor][room], same order as the brief
const char* room_names[NUM_FLOORS][ROOMS_PER_FLOOR] = {
    {"Entrance Lobby", "Reception", "Waiting Area", "Utility Room"},
    {"Lounge", "Archive", "Lab", "Meeting Room"},
    {"Stairwell Annex", "Security Vestibule", "Animal Room", "Control Room"},
    {"First Aid Station", "Medical Room", "Maintenance Corridor", "Office"},
    {"Security Room", "Train Boarding Platform", "Storage Room", "Observation Deck"}
};

void renderDashboard(int player_floor, int player_room, int energy, int time, int score,
    const char inventory[MAX_INVENTORY_CAPACITY], int inventory_count,
    int emergency_trips_left)
{
    cout << "\n================ STATUS ================\n";

    // where am i
    cout << "Location : F" << player_floor << "-R" << player_room;
    if (player_floor >= 1 && player_floor <= NUM_FLOORS &&
        player_room >= 1 && player_room <= ROOMS_PER_FLOOR) {
        cout << " (" << room_names[player_floor - 1][player_room - 1] << ")";
    }
    cout << "\n";

    // stats
    cout << "Energy   : " << energy << " / " << MAX_ENERGY << "\n";
    cout << "Time     : " << time << " / " << MAX_TIME << "\n";
    cout << "Score    : " << score << "\n";

    // bag
    cout << "Bag (" << inventory_count << "/" << MAX_INVENTORY_CAPACITY << "): ";
    if (inventory_count == 0) {
        cout << "empty";
    }
    for (int i = 0; i < inventory_count; i++) {
        cout << getEntityName(inventory[i]);
        if (i < inventory_count - 1) {
            cout << ", ";
        }
    }
    cout << "\n";

    cout << "Lift 2 emergency trips: " << emergency_trips_left << "\n";
    cout << "========================================\n";
}

void renderRoomDetails(int floor, int room,
    const char room_entities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
    int entity_count)
{

    if (floor < 1 || floor > NUM_FLOORS || room < 1 || room > ROOMS_PER_FLOOR) {
        cout << "Unknown room.\n";
        return;
    }

    int floor_index = floor - 1;
    int room_index = room - 1;

    cout << "\nYou are in F" << floor << "-R" << room
        << " (" << room_names[floor_index][room_index] << ")\n";

    if (entity_count == 0) {
        cout << "The room looks empty.\n";
        return;
    }

    cout << "You can see:\n";
    for (int i = 0; i < entity_count; i++) {
        char thing = room_entities[floor_index][room_index][i];
        cout << " - " << getEntityName(thing) << " (" << thing << ")\n";
    }
}

void renderAsciiBuilding(int player_floor, int player_room)
{
    // what's between room 1-2, 2-3, 3-4 on each floor (F1 first)
    // ' ' means no direct link (F1/F2 middle is lift 1)
    const char map_links[NUM_FLOORS][ROOMS_PER_FLOOR - 1] = {
        {'W', ' ', 'D'},   // F1
        {'D', ' ', 'D'},   // F2
        {'L', 'W', 'D'},   // F3
        {'W', '#', 'D'},   // F4
        {'D', 'D', 'W'}    // F5
    };

    cout << "\n";

    // top floor first, so the map looks like a building
    for (int floor = NUM_FLOORS; floor >= 1; floor--) {

        cout << "F" << floor << " | ";

        for (int room = 1; room <= ROOMS_PER_FLOOR; room++) {

            // mark the player's room
            if (floor == player_floor && room == player_room) {
                cout << "[ YOU ]";
            }
            else {
                cout << "  F" << floor << "-R" << room;
            }

            // link to the next room
            if (room < ROOMS_PER_FLOOR) {
                cout << "  " << map_links[floor - 1][room - 1] << " ";
            }
        }

        // lift 2 stops on F2, F4, F5
        cout << "  |";
        if (floor == 2 || floor == 4 || floor == 5) {
            cout << " L2";
        }
        cout << "\n";

        // stairs and lift 1 sit between floors
        if (floor == 5) cout << "S3\n";              // F4 <-> F5
        if (floor == 3) cout << "S2\n";              // F2 <-> F3
        if (floor == 2) cout << "S1                      L1\n";   // F1 <-> F2
    }
}
void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES])
{
    // names in the same order as state.h
    const char* objective_names[NUM_OBJECTIVES] = {
        "Master key collected",
        "Clue 4 found",
        "Clue 2 found",
        "Clue 7 found",
        "Cabinet unlocked",
        "Train key acquired",
        "Fuse collected",
        "Fuse installed",
        "Lift pass collected",
        "Lift 2 active",
        "Passenger rescued",
        "Cat carrier collected",
        "Cat secured",
        "Train prepared",
        "Mission won",
        "Mission failed"
    };

    cout << "\n=============== OBJECTIVES ===============\n";

    // skip won/failed, those show at the end
    for (int i = 0; i < OBJ_MISSION_WON; i++) {
        if (objectives[i]) {
            cout << "[x] ";
        }
        else {
            cout << "[ ] ";
        }
        cout << objective_names[i] << "\n";
    }
    cout << "==========================================\n";
}