#include "simulator.h"
#include "level.h"
#include "logger.h"
#include "viewer_iface.h"
#include <iostream>
using namespace std;

int main()
{
    // Player State Primitives
    int playerFloor = 1;
    int playerRoom = 1;
    int energy = INITIAL_ENERGY;
    int time = INITIAL_TIME;
    int score = INITIAL_SCORE;

    char inventory[MAX_INVENTORY_CAPACITY];
    int inventoryCount = 0;
    int emergencyTripsLeft = 0;

    // Objective State Booleans
    bool objectives[NUM_OBJECTIVES] = {false};

    // Building Connectivity & Entity Parallel Matrices
    char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1];
    char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR] = {0};

    const char *mapFile = "data/building.txt";
    const char *initStateFile = "data/initial-state.txt";

    if (!loadBuildingMap(mapFile, horizontalConn, roomEntities, entityCounts))
    {
        std::cerr << "Error: Could not load " << mapFile << "\n";
        return 1;
    }

    char choice;
    std::cout << "\n========== THE LAST TRAIN ==========\n";
    std::cout << "N. New Game\n";
    std::cout << "R. Resume Game\n";
    std::cout << "Q. Quit\n";
    std::cout << "Select an option: ";
    std::cin >> choice;

    // Convert lowercase to uppercase
    if (choice >= 'a' && choice <= 'z')
        choice = choice - 32;

    if (choice == 'N')
    {
        if (!loadInitialState(initStateFile, playerFloor, playerRoom, energy, time, score,
                              inventory, inventoryCount, objectives, emergencyTripsLeft))
        {
            std::cerr << "Error: Could not load " << initStateFile << "\n";
            return 1;
        }

        cout << "\nSelect your starting floor (1-" << NUM_FLOORS << "): ";
        cin >> playerFloor;

        cout << "Select your starting room (1-" << ROOMS_PER_FLOOR << "): ";
        cin >> playerRoom;

        if (!validateStartingRoom(playerFloor, playerRoom))
        {
            cout << "Invalid starting position!" << endl;
            return 1;
        }
    }

    else if (choice == 'R')
    {
        if (!loadGameStateFromFile(SAVE_FILE_NAME, playerFloor, playerRoom, energy, time, score,
                                   inventory, inventoryCount, objectives, emergencyTripsLeft,
                                   roomEntities, entityCounts))
        {
            std::cerr << "Error: Could not load " << SAVE_FILE_NAME << "\n";
            return 1;
        }
    }

    else if (choice == 'Q')
    {
        std::cout << "Exiting the game. Goodbye!\n";
        return 0;
    }

    else
    {
        std::cout << "Invalid option selected. Exiting.\n";
        return 1;
    }

    std::cout << "===========================================\n";
    std::cout << " THE LAST TRAIN - SIMULATION INITIALIZED   \n";
    std::cout << "===========================================\n";

    runSimulation(playerFloor, playerRoom, energy, time, score,
                  inventory, inventoryCount, objectives, emergencyTripsLeft,
                  horizontalConn, roomEntities, entityCounts);
                  if(objectives[OBJ_MISSION_WON] || objectives[OBJ_MISSION_FAILED])
                  {
                    writeGameSummaryReport("data/game_summary.txt", score, energy, time, objectives, objectives[OBJ_MISSION_WON]);
                  }

    return 0;
}
