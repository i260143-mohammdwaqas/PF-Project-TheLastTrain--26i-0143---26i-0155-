#include<iostream>
#include<fstream>
#include "logger.h"
#include "util.h"
#include "level.h"
using namespace std;

// File persistence for savegame.txt and game_summary.txt using primitive variables
bool saveGameStateToFile(const char* filename,
                         int playerFloor, int playerRoom,
                         int energy, int time, int score,
                         const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                         const bool objectives[NUM_OBJECTIVES], int emergencyTripsLeft,
                         const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                         const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
                         {
                            
                            //validte the values

                            //validate location
                            if(playerFloor<1 || playerFloor>NUM_FLOORS || playerRoom<1 || playerRoom>ROOMS_PER_FLOOR)
                            {
                                cout<<"Invalid Player location!"<<endl;
                                return false;
                            }

                            //validate invnentory count
                            if(inventoryCount<0 || inventoryCount>MAX_INVENTORY_CAPACITY)
                            {
                                cout<<"Invalid inventory!"<<endl;
                                return false;
                            }

                            //validate entity counts
                            for(int i=0; i<NUM_FLOORS; i++)
                            {
                                for(int j=0; j<ROOMS_PER_FLOOR; j++)
                                {
                                    if(entityCounts[i][j]<0 || entityCounts[i][j]>MAX_ENTITIES_PER_ROOM)
                                    {
                                        cout<<"Invalid entity count!"<<endl;
                                        return false;
                                    }
                                }
                            }

                            //validate resources
                            if(energy<0 || energy>MAX_ENERGY || time<0 || time>MAX_TIME || score<0)
                            {
                                cout<<"Invalid resources!"<<endl;
                                return false;
                            }

                            if(emergencyTripsLeft<0 || emergencyTripsLeft>3)
                            {
                                cout<<"Invalid emergency trips!"<<endl;
                                return false;
                            }

                            if(filename==nullptr)
                            {
                                cout<<"Invalid filename!"<<endl;
                                return false;
                            }


                            //open the file for writing
                            ofstream file(filename);

                            //check wether the file has opened succesfully
                            if(!file.is_open())
                            {
                                cout<<"couldn't save Game progress!"<<endl;
                                return false;
                            }

                     

                            


                            //write the information into the file

                            file<<"     Player's Location"<<endl;
                            file<<"Floor: "<<playerFloor<<endl;
                            file<<"Room: "<<playerRoom<<endl;


                            file<<"\nEnergy: "<<energy<<" "<<"Score: "<<score<<" "<<"Time: "<<time<<endl;


                            file<<"\n     Inventory"<<endl;
                            file<<"Inventory Count: "<<inventoryCount<<endl;  

                            for(int i=0; i<inventoryCount; i++)
                            {                                                             
                               file<<inventory[i]<<" ";
                            }
                            file<<endl;


                            file<<"\nEmergency trips remaining: "<<emergencyTripsLeft<<endl;

 
                            file<<"\n    Objective Flags"<<endl;
                            file<<"Master Key: "<<(objectives[OBJ_MASTER_KEY]? "True" : "False")<<endl;
                            file<<"Cabinet Unlocked: "<<(objectives[OBJ_CABINET_UNLOCKED]? "True" : "False")<<endl;
                            file<<"Cat Carrier: "<<(objectives[OBJ_CAT_CARRIER]? "True" : "False")<<endl;
                            file<<"Cat Secured: "<<(objectives[OBJ_CAT_SECURED]? "True" : "False")<<endl;
                            file<<"Clue Digit 2: "<<(objectives[OBJ_CLUE_2]? "True" : "False")<<endl;
                            file<<"Clue Digit 4: "<<(objectives[OBJ_CLUE_4]? "True" : "False")<<endl;
                            file<<"Clue Digit 7: "<<(objectives[OBJ_CLUE_7]? "True" : "False")<<endl;
                            file<<"Fuse Collected: "<<(objectives[OBJ_FUSE_COLLECTED]? "True" : "False")<<endl;
                            file<<"Fuse Installed: "<<(objectives[OBJ_FUSE_INSTALLED]? "True" : "False")<<endl;
                            file<<"Lift 2 Active: "<<(objectives[OBJ_LIFT2_ACTIVE]? "True" : "False")<<endl;
                            file<<"Lift Pass: "<<(objectives[OBJ_LIFT_PASS]? "True" : "False")<<endl;
                            file<<"Mission Failed: "<<(objectives[OBJ_MISSION_FAILED]? "True" : "False")<<endl;
                            file<<"Mission Won: "<<(objectives[OBJ_MISSION_WON]? "True" : "False")<<endl;
                            file<<"Passenger Rescued: "<<(objectives[OBJ_PASSENGER_RESCUED]? "True" : "False")<<endl;
                            file<<"Train Key: "<<(objectives[OBJ_TRAIN_KEY]? "True" : "False")<<endl;
                            file<<"Train Prepared: "<<(objectives[OBJ_TRAIN_PREPARED]? "True" : "False")<<endl;
                            


                            file<<"\n    Room Entities"<<endl;
                            for(int i=0; i<NUM_FLOORS; i++)
                            {   
                               
                                for(int j=0; j<ROOMS_PER_FLOOR; j++)
                                {
                                    file<<endl;
                                    file<<"Floor: "<<i+1<<" Room: "<<j+1<<endl;
                                    file<<"Entity count: "<<entityCounts[i][j]<<endl;
                                    file<<"Entities: ";
                                    for(int z=0; z<entityCounts[i][j]; z++)
                                    {
                                        
                                        file<<roomEntities[i][j][z]<<" ";
                                    }


                                }
                            }

                            file.close();

                            //check if fie was succesfully written
                            if(!file)
                            {
                                cout<<"Error! writing the Game Progress"<<endl;
                                return false;
                            }

                            return true;


                         }



 
bool loadGameStateFromFile(const char* filename,
                           int& playerFloor, int& playerRoom,
                           int& energy, int& time, int& score,
                           char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                           bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                           char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                           int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
                           {}


void writeGameSummaryReport(const char* filename,
                            int score, int remainingEnergy, int remainingTime,
                            const bool objectives[NUM_OBJECTIVES], bool victory)
                            {}                       