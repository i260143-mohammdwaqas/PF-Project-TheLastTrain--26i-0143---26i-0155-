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
                                cout<<"Couldn't save Game progress!"<<endl;
                                return false;
                            }         


                            //write the information into the file

                            file<<"     Coordinates"<<endl;
                            file<<"Player's Floor: "<<playerFloor<<endl;
                            file<<"Player's Room: "<<playerRoom<<endl;


                            file<<"\nEnergy: "<<energy<<" "<<"Score: "<<score<<" "<<"Time: "<<time<<endl;


                            file<<"\n"<<endl;
                            file<<"Inventory Count: "<<inventoryCount<<endl;  
                            file<<"Inventory: ";
                            for(int i=0; i<inventoryCount; i++)
                            {                                                             
                               file<<inventory[i]<<" ";
                            }
                            file<<endl;


                            file<<"\nEmergency Trips Left: "<<emergencyTripsLeft<<endl;

 
                            file<<"\n    Objective Flags"<<endl;
                            file<<"Master Key: "<<(objectives[OBJ_MASTER_KEY]? "1" : "0")<<endl;
                            file<<"Cabinet Unlocked: "<<(objectives[OBJ_CABINET_UNLOCKED]? "1" : "0")<<endl;
                            file<<"Cat Carrier: "<<(objectives[OBJ_CAT_CARRIER]? "1" : "0")<<endl;
                            file<<"Cat Secured: "<<(objectives[OBJ_CAT_SECURED]? "1" : "0")<<endl;
                            file<<"Clue Digit 2: "<<(objectives[OBJ_CLUE_2]? "1" : "0")<<endl;
                            file<<"Clue Digit 4: "<<(objectives[OBJ_CLUE_4]? "1" : "0")<<endl;
                            file<<"Clue Digit 7: "<<(objectives[OBJ_CLUE_7]? "1" : "0")<<endl;
                            file<<"Fuse Collected: "<<(objectives[OBJ_FUSE_COLLECTED]? "1" : "0")<<endl;
                            file<<"Fuse Installed: "<<(objectives[OBJ_FUSE_INSTALLED]? "1" : "0")<<endl;
                            file<<"Lift 2 Active: "<<(objectives[OBJ_LIFT2_ACTIVE]? "1" : "0")<<endl;
                            file<<"Lift Pass: "<<(objectives[OBJ_LIFT_PASS]? "1" : "0")<<endl;
                            file<<"Mission Failed: "<<(objectives[OBJ_MISSION_FAILED]? "1" : "0")<<endl;
                            file<<"Mission Won: "<<(objectives[OBJ_MISSION_WON]? "1" : "0")<<endl;
                            file<<"Passenger Rescued: "<<(objectives[OBJ_PASSENGER_RESCUED]? "1" : "0")<<endl;
                            file<<"Train Key: "<<(objectives[OBJ_TRAIN_KEY]? "1" : "0")<<endl;
                            file<<"Train Prepared: "<<(objectives[OBJ_TRAIN_PREPARED]? "1" : "0")<<endl;
                            


                            file<<"\n    Room Entities"<<endl;
                            for(int i=0; i<NUM_FLOORS; i++)
                            {   
                               
                                for(int j=0; j<ROOMS_PER_FLOOR; j++)
                                {
                                    file<<endl;
                                    file<<"Floor: "<<i+1<<" Room: "<<j+1<<endl;
                                    file<<"Entity Count: "<<entityCounts[i][j]<<endl;
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

                            return 1;


                         }



 
bool loadGameStateFromFile(const char* filename,
                           int& playerFloor, int& playerRoom,
                           int& energy, int& time, int& score,
                           char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                           bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                           char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                           int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
                           {

                            //validate file address stored in filename
                            if(filename==nullptr)
                            {
                                cout<<"Invalid filename!"<<endl;
                                return false;
                            }

                            //open the file
                            ifstream file(filename);
                            if(!file)
                            {
                                cout<<"Error! opening the Game Progress file"<<endl;
                                return false;
                            }

                            string line;
                            int pos, f=0, r=0;

                            //read ech line of the file
                            while(getline(file, line))
                            {
                                //search for keyWords

                                //search for floor and room
                                //stoi converts the char into integer, and substr extracts the substring from the line
                                pos=line.find("Player's Floor: ");
                                if(pos != string::npos)
                                {
                                    playerFloor=stoi(line.substr(pos+16));
                                }

                               pos=line.find("Player's Room: ");
                                if(pos != string::npos)
                                {
                                    playerRoom=stoi(line.substr(pos+15));
                                }

                                
                                //search for Enrgy, Time, Score
                                pos=line.find("Energy: ");
                                if(pos != string::npos)
                                {
                                    energy=stoi(line.substr(pos+8));
                                }

                                pos=line.find("Time: ");
                                if(pos != string::npos)
                                {
                                    time=stoi(line.substr(pos+6));
                                }

                                pos=line.find("Score: ");
                                if(pos != string::npos)
                                {
                                    score=stoi(line.substr(pos+7));;
                                }




                                //search for inventoryCount                
                                pos=line.find("Inventory Count: ");
                                if(pos!= string::npos)
                                {
                                    inventoryCount=stoi(line.substr(pos+17));
                                }

                                //validate inventory count
                                if(inventoryCount<0 || inventoryCount>MAX_INVENTORY_CAPACITY)
                                {
                                    cout<<"Invalid inventory count!"<<endl;
                                    return false;
                                }

                                pos=line.find("Inventory: ");
                                if(pos!= string::npos)                                   
                                { 
                                    for(int i=0; i<inventoryCount; i++)
                                    {
                                        inventory[i]=line[pos+11+i*2];   
                                    } 
                                }
 
    


                                //search for emergencyTripsLeft
                                pos=line.find("Emergency Trips Left: ");
                                if(pos != string::npos)
                                {
                                    emergencyTripsLeft=stoi(line.substr(pos+22));
                                }




                                //find objectives section
                                //set objectives 1 or 0 accordingly
                                pos=line.find("Master Key: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_MASTER_KEY]=stoi(line.substr(pos+12));                                   
                                }

                                pos=line.find("Cabinet Unlocked: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_CABINET_UNLOCKED]=stoi(line.substr(pos+18));                                   
                                }

                                pos=line.find("Cat Carrier: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_CAT_CARRIER]=stoi(line.substr(pos+13));                                   
                                }

                                pos=line.find("Cat Secured: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_CAT_SECURED]=stoi(line.substr(pos+13));                                   
                                }

                                pos=line.find("Clue Digit 2: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_CLUE_2]=stoi(line.substr(pos+14));                                   
                                }

                                pos=line.find("Clue Digit 4: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_CLUE_4]=stoi(line.substr(pos+14));                                   
                                }

                                pos=line.find("Clue Digit 7: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_CLUE_7]=stoi(line.substr(pos+14));                                   
                                }

                                pos=line.find("Fuse Collected: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_FUSE_COLLECTED]=stoi(line.substr(pos+16));                                   
                                }

                                pos=line.find("Fuse Installed: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_FUSE_INSTALLED]=stoi(line.substr(pos+16));                                   
                                }

                                pos=line.find("Lift 2 Active: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_LIFT2_ACTIVE]=stoi(line.substr(pos+15));                                   
                                }

                                pos=line.find("Lift Pass: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_LIFT_PASS]=stoi(line.substr(pos+11));                                   
                                }


                                pos=line.find("Mission Failed: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_MISSION_FAILED]=stoi(line.substr(pos+16));                                   
                                }


                                pos=line.find("Mission Won: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_MISSION_WON]=stoi(line.substr(pos+13));                                   
                                }


                                pos=line.find("Passenger Rescued: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_PASSENGER_RESCUED]=stoi(line.substr(pos+19));                                   
                                }


                                pos=line.find("Train Key: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_TRAIN_KEY]=stoi(line.substr(pos+11));                                   
                                }


                                pos=line.find("Train Prepared: ");
                                if(pos!= string::npos)
                                {
                                    objectives[OBJ_TRAIN_PREPARED]=stoi(line.substr(pos+16));                                   
                                }
                 
                                
        
        
                                //find floor, room and and its entityCount
                                pos=line.find("Floor: ");
                                if(pos != string::npos && line.find("Player's Floor: ")==string::npos)
                                {
                                    f=stoi(line.substr(pos+7));
                                }

                                pos=line.find("Room: ");
                                if(pos != string::npos && line.find("Player's Room: ")==string::npos)
                                {
                                    r=stoi(line.substr(pos+6));
                                }


                                pos=line.find("Entity Count: ");
                                if(pos != string::npos)
                                {
                                    if(f<1 || f>NUM_FLOORS || r<1 || r>ROOMS_PER_FLOOR)
                                    {
                                        cout<<"Invalid floor or room!"<<endl;
                                        return false;
                                    }

                                    entityCounts[f-1][r-1]=stoi(line.substr(pos+14));

                                    if(entityCounts[f-1][r-1]<0 || entityCounts[f-1][r-1]>MAX_ENTITIES_PER_ROOM)
                                    {
                                        cout<<"Invalid entity count!"<<endl;
                                        return false;
                                    }
                                }

  
                                //find entities
                                pos=line.find("Entities: ");
                                //run a a loop on the line, wheneve a symbol is found its stored 
                                if(pos != string::npos)
                                {
                                    for(int i=0; i<entityCounts[f-1][r-1]; i++)
                                    {
                                        //multiply i by 2 so that spaces are skipped
                                        roomEntities[f-1][r-1][i]=line[pos+10+i*2];
                                    }
                                }

                            }
                            file.close();

                            
                             //validate floor and room
                             if(playerFloor<1 || playerFloor>NUM_FLOORS || playerRoom<1 || playerRoom>ROOMS_PER_FLOOR)
                            {
                                cout<<"Invalid player coordinates!"<<endl;
                                return false;
                            }

                             //validate energy time and score;
                            if(energy<0 || time<0|| energy>MAX_ENERGY || time>MAX_TIME)
                            {
                                cout<<"Invalid resources!"<<endl;
                                return false;
                            }

                            //validate score
                            if(score<0)
                            {
                                cout<<"Invalid score!"<<endl;
                                return false;
                            }

                            //validate emrgencyTrips
                            if(emergencyTripsLeft<0 || emergencyTripsLeft>3)
                            {
                                cout<<"Invalid emergency trips!"<<endl;
                                return false;
                            }

                            
     



                            return true;

                           }


void writeGameSummaryReport(const char* filename,
                            int score, int remainingEnergy, int remainingTime,
                            const bool objectives[NUM_OBJECTIVES], bool victory)
                            {}                       