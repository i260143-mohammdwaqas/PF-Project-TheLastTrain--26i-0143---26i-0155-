#include<iostream>
#include<string>
#include<cctype>
#include<fstream>
#include "logger.h"
#include "util.h"
#include "level.h"
#include "physics.h"
using namespace std;

// File persistence for savegame.txt and game_summary.txt using primitive variables

//created my own function for converting string to integer, convertToInt?(to be moved to util.h)


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


                            //validate the arrays
                            if(inventory==nullptr)
                            {
                                cout<<"Invalid inventory data!"<<endl;
                                return false;
                            }

                            if(roomEntities==nullptr)
                            {
                                cout<<"Invalid room entities data!"<<endl;
                                return false;
                            }

                            if(entityCounts==nullptr)
                            {
                                cout<<"Invalid entity counts data!"<<endl;
                                return false;
                            }

                            if(objectives==nullptr)
                            {
                                cout<<"Invalid objectives data!"<<endl;
                                return false;
                            }

                            //vaidate the file address stored in filename
                            if(filename==nullptr)
                            {
                                cout<<"Invalid filename!"<<endl;
                                return false;
                            }

                            //validate inventory count
                            if(inventoryCount<0 || inventoryCount>MAX_INVENTORY_CAPACITY)
                            {
                                cout<<"Invalid inventory!"<<endl;
                                return false;
                            }

                             //validate the inventory symbol
                             for(int i=0; i<inventoryCount; i++)
                              if(!isValidInventory(inventory[i]))
                                {
                                    cout<<"Invalid inventory symbol!"<<endl;
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

                            //validate entity symbols
                            for(int i=0; i<NUM_FLOORS; i++)
                            {
                                for(int j=0; j<ROOMS_PER_FLOOR; j++)
                                {
                                    for(int k=0; k<entityCounts[i][j]; k++)
                                    {
                                        if(!isValidEntity(roomEntities[i][j][k]))
                                        {
                                            cout<<"Invalid entity symbol!"<<endl;
                                            return false;
                                        }
                                    }
                                }
                            }

                            //validate resources
                            if(energy<0 || energy>MAX_ENERGY || time<0 || time>MAX_TIME || score<0 || score>MAX_SCORE)
                            {
                                cout<<"Invalid resources!"<<endl;
                                return false;
                            }

                            if(emergencyTripsLeft<0 || emergencyTripsLeft>3)
                            {
                                cout<<"Invalid emergency trips!"<<endl;
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



                            file<<"\nEnergy: "<<energy<<"\nScore: "<<score<<"\nTime: "<<time<<endl;


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
                                    file<<"Floor: "<<i+1<<"\nRoom: "<<j+1<<endl;
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

                            return true;

                        }
   

//converts the string to integer, returns false if the string is not a valid integer              
bool convertToInt(const string& str, int& value)
{
    // Use std::stoi to convert string to integer, with error handling
    //size_t is used to store postions 
    size_t pos;

    //if there is an error in the conversion, it will throw an exception, which we can catch and return false
    try
    {
        value=stoi(str, &pos);

        //after finding pos we check whether the psotion + the remainign whitespaces is equal to the length of the string, if not then it means there are some non-whitespace characters after the integer, which means the string is not a valid integer
        //we use isspace to check for whitespace characters
        // the loop continues until it encounters a non-whitespace character or reaches the end of the string
        while(pos<str.length() && isspace(static_cast<unsigned char>(str[pos])))
        {
            pos++;
        }

        if(pos<str.length())
        {
            return false;
        }
    
        return true;
    }
    catch(...)
    {
        return false;
    }
}





 
bool loadGameStateFromFile(const char* filename,
                           int& playerFloor, int& playerRoom,
                           int& energy, int& time, int& score,
                           char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                           bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                           char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                           int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
                           {
                            //check wether the arrays are valid or not, if not return false
                            if(inventory==nullptr)
                            {
                                cout<<"Invalid inventory data!"<<endl;
                                return false;
                            }

                            if(objectives==nullptr)
                            {
                                cout<<"Invalid objectives data!"<<endl;
                                return false;
                            }

                            if(roomEntities==nullptr)
                            {
                                cout<<"Invalid room entities data!"<<endl;
                                return false;
                            }

                            if(entityCounts==nullptr)
                            {
                                cout<<"Invalid entity counts data!"<<endl;
                                return false;
                            }

                            //validate file address stored in filename
                            if(filename==nullptr)
                            {
                                cout<<"Invalid filename!"<<endl;
                                return false;
                            }

                            
                            //initialize variables to -1 so that we can check wether the values have been set or not
                            inventoryCount=-1;
                            emergencyTripsLeft=-1;
                            playerFloor=-1;
                            playerRoom=-1;
                            energy=-1;
                            time=-1;
                            score=-1;


                            //checks wether the inventory section has been read or not
                            bool readingInventory=false;

                           //inialize all objectives to false
                            for(int i=0; i<NUM_OBJECTIVES; i++)
                               objectives[i]=false;

                            //check wether the objectives have been read or not,
                            bool readingObjectives[NUM_OBJECTIVES]={false};

                            //check wther the entitiees section for each room has been read or not, if not then return false
                            bool readingEntities[NUM_FLOORS][ROOMS_PER_FLOOR]={false};
                        
                            //initialize the room entities to none
                            for(int i=0; i<NUM_FLOORS; i++) 
                            {
                                for(int j=0; j<ROOMS_PER_FLOOR; j++)
                                {
                                    //initialize entity counts to -1, so that later we can check wether the record exists
                                    entityCounts[i][j]=-1;

                                    for(int k=0; k<MAX_ENTITIES_PER_ROOM; k++)
                                    {
                                    roomEntities[i][j][k]=ENT_NONE;
                                    }
                                }
                            }


                            //open the file
                            ifstream file(filename);
                            if(!file)
                            {
                                cout<<"Error! opening the Game Progress file"<<endl;
                                return false;
                            }

                            //inner loop variables
                            string line;
                            string::size_type pos;
                            int f=0, r=0, flag;
                            
                     
                            //read ech line of the file
                            while(getline(file, line))
                            {
                                //search for keyWords

                                //search for floor and room
                                //stoi converts the char into integer, and substr extracts the substring from the line
                                pos=line.find("Player's Floor: ");
                                if(pos != string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+16), playerFloor))
                                    {
                                        cout<<"Invalid Player Floor!"<<endl;
                                        return false;
                                    }
                                }

                               pos=line.find("Player's Room: ");
                                if(pos != string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+15), playerRoom))
                                    {
                                        cout<<"Invalid Player Room!"<<endl;
                                        return false;
                                    }
                                }

                                
                                //search for Enrgy, Time, Score
                                pos=line.find("Energy: ");
                                if(pos != string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+8), energy))
                                    {
                                        cout<<"Invalid Energy!"<<endl;
                                        return false;
                                    }
                                }

                                pos=line.find("Time: ");
                                if(pos != string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+6), time))
                                    {
                                        cout<<"Invalid Time!"<<endl;
                                        return false;
                                    }
                                }

                                pos=line.find("Score: ");
                                if(pos != string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+7), score))
                                    {
                                        cout<<"Invalid Score!"<<endl;
                                        return false;
                                    }
                                }




                                //search for inventoryCount                
                                pos=line.find("Inventory Count: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+17), inventoryCount))
                                    {
                                        cout<<"Invalid inventory count!"<<endl;
                                        return false;
                                    }

                                   //validate inventory count
                                   if(inventoryCount<0 || inventoryCount>MAX_INVENTORY_CAPACITY)
                                   {
                                      cout<<"Invalid inventory count!"<<endl;
                                      return false;
                                   }
                                }
                                

                                pos=line.find("Inventory: ");
                                if(pos!= string::npos)                                   
                                { 
                                   //check wether the characters are as much as inventory
                                   if(inventoryCount>0 && line.length()<=pos+11+2*(inventoryCount-1))
                                   {
                                      cout<<"Invalid inventory data!"<<endl;
                                      return false;
                                    }


                                    for(int i=0; i<inventoryCount; i++)
                                    {
                                        inventory[i]=line[pos+11+i*2]; 

                                        //validate the inventory symbol
                                        if(!isValidInventory(inventory[i]))
                                        {
                                            cout<<"Invalid inventory symbol!"<<endl;
                                            return false;
                                        }  
                                    } 

                                    readingInventory=true;

                                }
 
    


                                //search for emergencyTripsLeft
                                pos=line.find("Emergency Trips Left: ");
                                if(pos != string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+22), emergencyTripsLeft))
                                    {
                                        cout<<"Invalid Emergency Trips Left!"<<endl;
                                        return false;
                                    }
                                }




                                //find objectives section
                                //validate wether the values are binary
                                //set objectives 1 or 0 accordingly
                                pos=line.find("Master Key: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+12), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }

                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_MASTER_KEY]=flag;
                                    readingObjectives[OBJ_MASTER_KEY]=true;

                                }

                                pos=line.find("Cabinet Unlocked: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+18), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }

                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_CABINET_UNLOCKED]=flag;
                                    readingObjectives[OBJ_CABINET_UNLOCKED]=true;
                                }

                                pos=line.find("Cat Carrier: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+13), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }

                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_CAT_CARRIER]=flag;
                                    readingObjectives[OBJ_CAT_CARRIER]=true;
                                }

                                pos=line.find("Cat Secured: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+13), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }

                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_CAT_SECURED]=flag;
                                    readingObjectives[OBJ_CAT_SECURED]=true;
                                }

                                pos=line.find("Clue Digit 2: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+14), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }

                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_CLUE_2]=flag;
                                    readingObjectives[OBJ_CLUE_2]=true;
                                }

                                pos=line.find("Clue Digit 4: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+14), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_CLUE_4]=flag;                                  
                                    readingObjectives[OBJ_CLUE_4]=true;
                                }

                                pos=line.find("Clue Digit 7: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+14), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }

                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_CLUE_7]=flag;                                   
                                    readingObjectives[OBJ_CLUE_7]=true;
                                }

                                pos=line.find("Fuse Collected: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+16), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }

                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_FUSE_COLLECTED]=flag;                                   
                                    readingObjectives[OBJ_FUSE_COLLECTED]=true;
                                }

                                pos=line.find("Fuse Installed: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+16), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }

                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_FUSE_INSTALLED]=flag;                                   
                                    readingObjectives[OBJ_FUSE_INSTALLED]=true;
                                }

                                pos=line.find("Lift 2 Active: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+15), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }

                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_LIFT2_ACTIVE]=flag;                                   
                                    readingObjectives[OBJ_LIFT2_ACTIVE]=true;
                                }

                                pos=line.find("Lift Pass: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+11), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }

                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_LIFT_PASS]=flag;                                   
                                    readingObjectives[OBJ_LIFT_PASS]=true;
                                }


                                pos=line.find("Mission Failed: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+16), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }

                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_MISSION_FAILED]=flag;                                   
                                    readingObjectives[OBJ_MISSION_FAILED]=true;
                                }


                                pos=line.find("Mission Won: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+13), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }

                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_MISSION_WON]=flag;                                   
                                    readingObjectives[OBJ_MISSION_WON]=true;
                                }


                                pos=line.find("Passenger Rescued: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+19), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_PASSENGER_RESCUED]=flag;
                                    readingObjectives[OBJ_PASSENGER_RESCUED]=true;
                                }


                                pos=line.find("Train Key: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+11), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }

                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_TRAIN_KEY]=flag;
                                    readingObjectives[OBJ_TRAIN_KEY]=true;
                                }


                                pos=line.find("Train Prepared: ");
                                if(pos!= string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+16), flag))
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }

                                    if(flag!=1 && flag!=0)
                                    {
                                        cout<<"Invalid objectives data!"<<endl;
                                        return false;
                                    }
                                    
                                    objectives[OBJ_TRAIN_PREPARED]=flag;
                                    readingObjectives[OBJ_TRAIN_PREPARED]=true;
                                }
                 
                                
        
        
                                //find floor, room and and its entityCount
                                pos=line.find("Floor: ");
                                if(pos != string::npos && line.find("Player's Floor: ")==string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+7), f))
                                    {
                                        cout<<"Invalid floor data!"<<endl;
                                        return false;
                                    }
                                }

                                pos=line.find("Room: ");
                                if(pos != string::npos && line.find("Player's Room: ")==string::npos)
                                {
                                    if(!convertToInt(line.substr(pos+6), r))
                                    {
                                        cout<<"Invalid room data!"<<endl;
                                        return false;
                                    }
                                }


                                pos=line.find("Entity Count: ");
                                if(pos != string::npos)
                                {
                                    if(f<1 || f>NUM_FLOORS || r<1 || r>ROOMS_PER_FLOOR)
                                    {
                                        cout<<"Invalid floor or room!"<<endl;
                                        return false;
                                    }

                                    if(!convertToInt(line.substr(pos+14), entityCounts[f-1][r-1]))
                                    {
                                        cout<<"Invalid entity count!"<<endl;
                                        return false;
                                    }

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
                                    //validate floor and room
                                    if(f<1 || f>NUM_FLOORS || r<1 || r>ROOMS_PER_FLOOR)
                                    {
                                        cout<<"Invalid floor or room!"<<endl;
                                        return false;
                                    }

                                    //validate that entities are equal to entity counts
                                    //2*(entityCounts[f-1][r-1]-1) includes spaces as weel as entity characters
                                    if(entityCounts[f-1][r-1]>0 && line.length()<=pos+10+2*(entityCounts[f-1][r-1]-1))
                                    {
                                        cout<<"Invalid entity data!"<<endl;
                                        return false;
                                    }

                                    for(int i=0; i<entityCounts[f-1][r-1]; i++)
                                    {                                        
                                        //multiply i by 2 so that spaces are skipped
                                        roomEntities[f-1][r-1][i]=line[pos+10+i*2];

                                        //validate that the entity is valid
                                        if(!isValidEntity(roomEntities[f-1][r-1][i]))
                                        {
                                            cout<<"Invalid Entity symbol!"<<endl;
                                            return false;
                                        }
                                    }

                                    
                                    readingEntities[f-1][r-1]=true;
                                }

                            }

                            //check if there was an error opening the file
                            if(file.bad())
                            {
                                cout<<"Error! reading the Game Progress file"<<endl;
                                return false;
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
                            if(score<0 || score>MAX_SCORE)
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

                            //check wether there wwere msiisng fields
                            
                            if(readingInventory==false || inventoryCount==-1 )
                            {
                                cout<<"Missing inventory data!"<<endl;
                                return false;
                            }
                            for(int i=0; i<NUM_FLOORS; i++)
                            {
                                for(int j=0; j<ROOMS_PER_FLOOR; j++)
                                if(entityCounts[i][j]==-1 || readingEntities[i][j]==false)
                                {
                                    cout<<"Missing entity data for floor "<<i+1<<" room "<<j+1<<endl;
                                    return false;
                                }
                            }

                            for(int i=0; i<NUM_OBJECTIVES; i++)
                            {
                               if(readingObjectives[i]==false)
                                {
                                   cout<<"Missing objective data!"<<endl;
                                   return false;
                                }
                            }

                            if(playerFloor==-1 || playerRoom==-1 || energy==-1 || time==-1 || score==-1 || emergencyTripsLeft==-1)
                            {
                                cout<<"Missing player data!"<<endl;
                                return false;
                            }




                            return true;

                           }


void writeGameSummaryReport(const char* filename,
                            int score, int remainingEnergy, int remainingTime,
                            const bool objectives[NUM_OBJECTIVES], bool victory)
                            {
                                //check if resources and other variables contain correct values
                                if(score<0 || score> MAX_SCORE || remainingEnergy<0 || remainingEnergy>MAX_ENERGY || remainingTime<0 || remainingTime>MAX_TIME)
                                {
                                    cout<<"Report could not be written, Invalid data!"<<endl;
                                    return;

                                }

                                if(objectives==nullptr)
                                {
                                    cout<<"Report could not be written, Invalid objectives data!"<<endl;
                                    return;
                                }


                                //check wether folder name is incorrect
                                if(filename==nullptr)
                                {
                                    cout<<"Report could not be wrtten, Invalid filename!"<<endl;
                                    return;
                                }

                                //open the file for writing
                                ofstream file(filename);
                                //check whether the file opended succesfully
                                if(!file.is_open())
                                {
                                    cout<<"Report file could not be opened!"<<endl;
                                    return;
                                }

                                file<<"    THE LAST TRAIN - GAME SUMMARY"<<endl;
                                
                                file<<"\nMission Result: "<<(victory? "Victory" : "Failure")<<endl;
                                file<<"Final Score: "<<score<<endl;
                                file<<"Remaining Energy: "<<remainingEnergy<<endl;
                                file<<"Remaining Time: "<<remainingTime<<endl;

                                file<<"      Objectives"<<endl;
                                file<<"Master Key: "<<(objectives[OBJ_MASTER_KEY]? "Completed" : "Not Completed")<<endl;
                                file<<"Cabinet Unlocked: "<<(objectives[OBJ_CABINET_UNLOCKED]? "Completed" : "Not Completed")<<endl;
                                file<<"Cat Carrier: "<<(objectives[OBJ_CAT_CARRIER]? "Completed" : "Not Completed")<<endl;
                                file<<"Cat Secured: "<<(objectives[OBJ_CAT_SECURED]? "Completed" : "Not Completed")<<endl;
                                file<<"Clue Digit 2: "<<(objectives[OBJ_CLUE_2]? "Completed" : "Not Completed")<<endl;
                                file<<"Clue Digit 4: "<<(objectives[OBJ_CLUE_4]? "Completed" : "Not Completed")<<endl;
                                file<<"Clue Digit 7: "<<(objectives[OBJ_CLUE_7]? "Completed" : "Not Completed")<<endl;
                                file<<"Fuse Collected: "<<(objectives[OBJ_FUSE_COLLECTED]? "Completed" : "Not Completed")<<endl;
                                file<<"Fuse Installed: "<<(objectives[OBJ_FUSE_INSTALLED]? "Completed" : "Not Completed")<<endl;
                                file<<"Lift 2 Active: "<<(objectives[OBJ_LIFT2_ACTIVE]? "Completed" : "Not Completed")<<endl;
                                file<<"Lift Pass: "<<(objectives[OBJ_LIFT_PASS]? "Completed" : "Not Completed")<<endl;
                                file<<"Mission Failed: "<<(objectives[OBJ_MISSION_FAILED]? "Completed" : "Not Completed")<<endl;
                                file<<"Mission Won: "<<(objectives[OBJ_MISSION_WON]? "Completed" : "Not Completed")<<endl;
                                file<<"Passenger Rescued: "<<(objectives[OBJ_PASSENGER_RESCUED]? "Completed" : "Not Completed")<<endl;
                                file<<"Train Key: "<<(objectives[OBJ_TRAIN_KEY]? "Completed" : "Not Completed")<<endl;
                                file<<"Train Prepared: "<<(objectives[OBJ_TRAIN_PREPARED]? "Completed" : "Not Completed")<<endl;

                                file.close();

                                if(!file)
                                {
                                    cout<<"Report could not be written!"<<endl;
                                    return;
                                }

                                cout<<"Game Summary Report written to "<<filename<<endl;

                            }                      