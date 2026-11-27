#include "logger.h"
#include "util.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;


bool saveGameStateToFile(const char* file_name,
    int player_floor, int player_room,
    int energy, int time, int score,
    const char inventory[MAX_INVENTORY_CAPACITY], int inventory_count,
    const bool objectives[NUM_OBJECTIVES], int emergency_trips_left,
    const char room_entities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
    const int entity_counts[NUM_FLOORS][ROOMS_PER_FLOOR])
{

    fstream save_file(file_name, ios::out);


    if (!save_file.is_open()) {
        cout << "Error: could not save to " << file_name << "\n";
        return false;
    }


    save_file << "FLOOR " << player_floor << "\n";
    save_file << "ROOM " << player_room << "\n";


    save_file << "ENERGY " << energy << "\n";
    save_file << "TIME " << time << "\n";
    save_file << "SCORE " << score << "\n";


    save_file << "INVENTORY " << inventory_count;
    for (int i = 0; i < inventory_count; i++) {
        save_file << " " << inventory[i];
    }
    save_file << "\n";


    save_file << "EMERGENCY_TRIPS " << emergency_trips_left << "\n";


    save_file << "OBJECTIVES";
    for (int i = 0; i < NUM_OBJECTIVES; i++) {
        save_file << " " << objectives[i];
    }
    save_file << "\n";

    save_file << "ROOMS\n";
    for (int floor_index = 0; floor_index < NUM_FLOORS; floor_index++) {
        for (int room_index = 0; room_index < ROOMS_PER_FLOOR; room_index++) {

            int item_count = entity_counts[floor_index][room_index];


            save_file << "F" << floor_index + 1 << "-R" << room_index + 1
                << " " << item_count;


            for (int i = 0; i < item_count; i++) {
                save_file << " " << room_entities[floor_index][room_index][i];
            }
            save_file << "\n";
        }
    }


    save_file.close();
    return true;
}


bool loadGameStateFromFile(const char* file_name,
    int& player_floor, int& player_room,
    int& energy, int& time, int& score,
    char inventory[MAX_INVENTORY_CAPACITY], int& inventory_count,
    bool objectives[NUM_OBJECTIVES], int& emergency_trips_left,
    char room_entities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
    int entity_counts[NUM_FLOORS][ROOMS_PER_FLOOR])
{

    fstream load_file(file_name, ios::in);
    if (!load_file.is_open()) {
        cout << "No saved game found.\n";
        return false;
    }

    string label;


    int temp_floor, temp_room, temp_energy, temp_time, temp_score;
    int temp_inventory_count, temp_trips;
    char temp_inventory[MAX_INVENTORY_CAPACITY];
    bool temp_objectives[NUM_OBJECTIVES];
    char temp_room_entities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int temp_entity_counts[NUM_FLOORS][ROOMS_PER_FLOOR];


    load_file >> label >> temp_floor;
    if (!load_file || label != "FLOOR" || temp_floor < 1 || temp_floor > NUM_FLOORS) {
        cout << "Save file is damaged.\n";
        return false;
    }
    load_file >> label >> temp_room;
    if (!load_file || label != "ROOM" || temp_room < 1 || temp_room > ROOMS_PER_FLOOR) {
        cout << "Save file is damaged.\n";
        return false;
    }


    load_file >> label >> temp_energy;
    if (!load_file || label != "ENERGY" || temp_energy < 0 || temp_energy > MAX_ENERGY) {
        cout << "Save file is damaged.\n";
        return false;
    }
    load_file >> label >> temp_time;
    if (!load_file || label != "TIME" || temp_time < 0 || temp_time > MAX_TIME) {
        cout << "Save file is damaged.\n";
        return false;
    }
    load_file >> label >> temp_score;
    if (!load_file || label != "SCORE" || temp_score < 0) {
        cout << "Save file is damaged.\n";
        return false;
    }


    load_file >> label >> temp_inventory_count;
    if (!load_file || label != "INVENTORY" ||
        temp_inventory_count < 0 || temp_inventory_count > MAX_INVENTORY_CAPACITY) {
        cout << "Save file is damaged.\n";
        return false;
    }
    for (int i = 0; i < temp_inventory_count; i++) {
        load_file >> temp_inventory[i];
    }


    load_file >> label >> temp_trips;
    if (!load_file || label != "EMERGENCY_TRIPS" || temp_trips < 0 || temp_trips > 3) {
        cout << "Save file is damaged.\n";
        return false;
    }


    load_file >> label;
    if (!load_file || label != "OBJECTIVES") {
        cout << "Save file is damaged.\n";
        return false;
    }
    for (int i = 0; i < NUM_OBJECTIVES; i++) {
        int flag_value;
        load_file >> flag_value;
        temp_objectives[i] = (flag_value == 1);
    }


    load_file >> label;
    if (!load_file || label != "ROOMS") {
        cout << "Save file is damaged.\n";
        return false;
    }
    for (int floor_index = 0; floor_index < NUM_FLOORS; floor_index++) {
        for (int room_index = 0; room_index < ROOMS_PER_FLOOR; room_index++) {

            int item_count;
            load_file >> label >> item_count;   // label = "F1-R1" etc
            if (!load_file || item_count < 0 || item_count > MAX_ENTITIES_PER_ROOM) {
                cout << "Save file is damaged.\n";
                return false;
            }
            temp_entity_counts[floor_index][room_index] = item_count;

            for (int i = 0; i < item_count; i++) {
                load_file >> temp_room_entities[floor_index][room_index][i];
            }
        }
    }


    if (!load_file) {
        cout << "Save file is damaged.\n";
        return false;
    }
    load_file.close();


    player_floor = temp_floor;
    player_room = temp_room;
    energy = temp_energy;
    time = temp_time;
    score = temp_score;
    emergency_trips_left = temp_trips;

    inventory_count = temp_inventory_count;
    for (int i = 0; i < temp_inventory_count; i++) {
        inventory[i] = temp_inventory[i];
    }
    for (int i = 0; i < NUM_OBJECTIVES; i++) {
        objectives[i] = temp_objectives[i];
    }
    for (int floor_index = 0; floor_index < NUM_FLOORS; floor_index++) {
        for (int room_index = 0; room_index < ROOMS_PER_FLOOR; room_index++) {
            entity_counts[floor_index][room_index] = temp_entity_counts[floor_index][room_index];
            for (int i = 0; i < temp_entity_counts[floor_index][room_index]; i++) {
                room_entities[floor_index][room_index][i] = temp_room_entities[floor_index][room_index][i];
            }
        }
    }

    return true;
}


void writeGameSummaryReport(const char* file_name,
    int score, int remaining_energy, int remaining_time,
    const bool objectives[NUM_OBJECTIVES], bool victory)
{

    fstream summary_file(file_name, ios::out);
    if (!summary_file.is_open()) {
        cout << "Error: could not write summary to " << file_name << "\n";
        return;
    }


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


    summary_file << "===== THE LAST TRAIN: GAME SUMMARY =====\n";
    if (victory) {
        summary_file << "Result: VICTORY\n";
    }
    else {
        summary_file << "Result: DEFEAT\n";
    }
    summary_file << "Final score: " << score << " / 520\n";
    summary_file << "Energy left: " << remaining_energy << "\n";
    summary_file << "Time left: " << remaining_time << "\n\n";


    summary_file << "Objectives:\n";
    int completed_count = 0;
    for (int i = 0; i < OBJ_MISSION_WON; i++) {
        if (objectives[i]) {
            summary_file << "[x] ";
            completed_count++;
        }
        else {
            summary_file << "[ ] ";
        }
        summary_file << objective_names[i] << "\n";
    }
    summary_file << "Completed: " << completed_count << " / " << OBJ_MISSION_WON << "\n";


    summary_file.close();
}