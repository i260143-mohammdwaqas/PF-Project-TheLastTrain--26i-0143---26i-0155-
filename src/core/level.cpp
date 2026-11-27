#include<iostream>
#include "level.h"
#include "util.h"
#include "logger.h"
#include<fstream>
#include<string>
using namespace std;

// Modular implementation using primitive arrays for map parsing and verification

//horizental conn issue



bool loadBuildingMap(const char* filename,
                     char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                     char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                     int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
                   {
                        ifstream file(filename);

                        //file validation
                        if (!file.is_open()) 
                            return false;


                        int lines=0;
                        string line[100];

                        //read the line one at a time and store it in a string array
                        while(lines<100 && getline(file, line[lines]))
                        lines++;




                      //initializing horizentalCon
                      for(int i=0; i<NUM_FLOORS; i++)
                      for(int j=0; j<ROOMS_PER_FLOOR-1; j++)
                      horizontalConn[i][j]=' ';


                        // initializing entity counts to zero
                        for(int i=0; i<NUM_FLOORS; i++)
                        {
                            for(int j=0; j<ROOMS_PER_FLOOR; j++)
                            {
                                entityCounts[i][j]=0;
                            }   
                        }


                        size_t posR[ROOMS_PER_FLOOR];
                        bool positionsfound=false;

                        //calculating Floor positions, helps in claculating room positions for entities
                        for(int i=0; i<lines; i++)
                        {
                            int r=0;
                            for(size_t j=0 ; j<line[i].length(); j++)
                             {

                              if(line[i][j]=='F')
                              {
                                if(r>=ROOMS_PER_FLOOR)
                                 return false;

                                posR[r]=j;
                                r++;
                              }

                            }

                            if(r==ROOMS_PER_FLOOR)
                            {
                                positionsfound=true;
                                break;
                            }
                        }

                         if(!positionsfound)
                            return false;


                         int f=-1, temp=-1;

                       //read linearray character by charater
                       for(int i=0; i<lines; i++)
                        {
                            //r stores room index
                            int entityCount=0, r=0;


                            //calculate floor number, helps in storing room connections in the correct floor and room
                            //but first check wether the line has all the required F's for a floor, if not skip the line(it could be another line containing f)
                            int countF = 0;

                           for(size_t j = 0; j < line[i].length(); j++)
                           {
                             if(line[i][j] == 'F')
                             countF++;
                            }

                             if(countF==ROOMS_PER_FLOOR)
                                {
                                size_t pos=line[i].find("F");
                                //subtracting 0 char to get integer;
                                 f = line[i][pos + 1] - '0'-1;

                                 //storing the line number in temp
                                 temp=i;
                                  if(f<0 || f>=NUM_FLOORS)
                                    return false;    

                            //R stores the room number

                            int R = 0;

                            //storing room connections in the horizontalConn array
                            //room boundaries exclude lift 2 but we need to accout for lift 1
                            for(size_t j = posR[0]; j<posR[ROOMS_PER_FLOOR - 1]; j++)
                            {
                                if(line[i][j] == 'W' || line[i][j] == 'D' ||
                                    line[i][j] == '#' || line[i][j] == 'L')
                                    {


                                    //j++ ensures that next charater is skipped if it is L1 
                                     
                                    if(line[i][j] == 'L' && j + 1 < line[i].length()
                                      && (line[i][j + 1] == '1'))
                                     {
                                        horizontalConn[f][R]=' ';
                                         R++;
                                         j++;
                                      }

                                    else
                                    {     
                                     if(R >= ROOMS_PER_FLOOR - 1)
                                     return false;  
                                                
                                    horizontalConn[f][R] = line[i][j];
                                    R++;  
                                    }

                                    
                                    }
                             }

                             //validate the 
                             if(R != ROOMS_PER_FLOOR - 1)
                                   return false;
                            }
                        //storing entities in the roomEntities array
                         if(i==temp+1)
                         {
                            for(size_t j=0; j<line[i].length(); j++)
                            {
                            //check if end of the room has come
                             if(r<ROOMS_PER_FLOOR - 1 && posR[r+1]==j)
                                   {
                                    entityCount=0;
                                    r++;}  

                             if(line[i][j]=='<' || line[i][j]==')' || line[i][j]=='\\' || line[i][j]== '4' || line[i][j]=='*' || line[i][j]=='~' || line[i][j]==';' || line[i][j]=='(' || line[i][j]=='^' || line[i][j]=='2' || line[i][j]=='$' || line[i][j]=='%' || line[i][j]=='&' || line[i][j]=='@'|| line[i][j]=='!' || line[i][j]=='7')
                                {
                                     if(entityCount>=MAX_ENTITIES_PER_ROOM)
                                       return false;

                                    entityCount++;
                                    //room entities srores items present in the particular room and  entityCounts stores the amount of them in a room
                                    roomEntities[f][r][entityCount-1]=line[i][j];
                                    entityCounts[f][r]=entityCount;

                                }


                            }
                       }
                    }


                    return true;
				}
                        
                        

bool loadInitialState(const char* filename,
                      int& playerFloor, int& playerRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft)
                      {

                        // Open the file for reading
                        ifstream file(filename);
                        
                        //file validation
                        if (!file.is_open()) 
                            return false;
                         
                        playerFloor=0;
                        playerRoom=0;
                        energy=0;
                        time=0;
                        score=0;
                        inventoryCount=0;
                        emergencyTripsLeft=0;

                        bool energyFound = false;
                        bool timeFound = false;
                        bool scoreFound = false;
                        bool startRoomFound = false;

                        for (int i = 0; i < NUM_OBJECTIVES; i++) {
                            objectives[i] = 0;
                        }
                        
                        for (int i = 0; i < MAX_INVENTORY_CAPACITY; i++) {
                            inventory[i] = '\0';
                        }

                        string line;
                        size_t pos;
                     
                        while(getline(file, line))
                        {
                                string temp;


                                pos = line.find("START_ROOM: ");
                                if(pos != string::npos)
                                {
                                   temp = line.substr(pos + 12);

                                   if(temp != "PLAYER_SELECTS")
                                     return false;

                                    startRoomFound = true;
}


                                pos=line.find("INITIAL_ENERGY: ");
                                if(pos!=string::npos)
                                {
                                     temp = line.substr(pos+16);
                                   
                                        if(!convertToInt(temp, energy))
                                            return false;

                                        if(energy<0 || energy>MAX_ENERGY)
                                            return false;
                                        energyFound = true;
                                    
                                }

                                pos=line.find("INITIAL_TIME: ");
                                if(pos!=string::npos)
                                {
                                    temp = line.substr(pos+14);
                                  
                                        if(!convertToInt(temp, time))
                                            return false;

                                        if(time<0 || time>MAX_TIME)
                                            return false;
                                        timeFound = true;
                                }

                                pos=line.find("INITIAL_SCORE: ");
                                if(pos!=string::npos)
                                {
                                    temp  = line.substr(pos+15);
                                    
                                        if(!convertToInt(temp, score))
                                            return false;
                                        if(score<0)
                                            return false;
                                        scoreFound = true;
                                }

                                pos=line.find("PASSENGER_RESCUED: ");
                                if(pos!=string::npos)
                                {
                                    temp=line.substr(pos+19);
                                     if(temp=="YES" )
                                     {                                     
                                        objectives[OBJ_PASSENGER_RESCUED] = true;
                                     }

                                     else if(temp=="NO" )
                                     {
                                        objectives[OBJ_PASSENGER_RESCUED] = false;
                                     }
                                    else
                                    {
                                        return false;
                                    }
                                }
                                
                                pos=line.find("CAT_RESCUED: ");
                                if(pos!=string::npos)
                                {
                                    temp=line.substr(pos+13);
                                    if(temp=="YES" )
                                    {                                    
                                        objectives[OBJ_CAT_SECURED] = true;
                                    }

                                    else if(temp=="NO" )
                                    {
                                        objectives[OBJ_CAT_SECURED] = false;
                                    }
                                    else
                                    {
                                        return false;
                                    }
                                }

                                pos=line.find("CLUES_COLLECTED: ");
                                if(pos!=string::npos)
                                {
                                    temp = line.substr(pos + 17);

                                    if(temp.find("4")!= string::npos)
                                    {
                                        objectives[OBJ_CLUE_4]=true;
                                    }
                                    if(temp.find("2")!= string::npos)
                                    {
                                        objectives[OBJ_CLUE_2]=true;
                                    }
                                    if(temp.find("7")!= string::npos)
                                    {
                                        objectives[OBJ_CLUE_7]=true;
                                    }
                                }

                                pos=line.find("CABINET_UNLOCKED: ");
                                if(pos!=string::npos)
                                {
                                temp=line.substr(pos+18);
                                if(temp=="YES" )
                                {
                                    objectives[OBJ_CABINET_UNLOCKED]=true;
                                }
                                else if(temp=="NO")
                                {
                                    objectives[OBJ_CABINET_UNLOCKED]=false;
                                }
                                else
                                {
                                    return false;
                                }
                                
                              }

 
                                pos=line.find("TRAIN_KEY_COLLECTED: ");
                               if(pos!=string::npos)
                                {
                                temp=line.substr(pos+21);
                                if(temp=="YES" )
                                {
                                    objectives[OBJ_TRAIN_KEY]=true;
                                 }
                                 else if(temp=="NO" )
                                 {
                                    objectives[OBJ_TRAIN_KEY]=false;
                                 }
                                 else
                                 {
                                     return false;
                                 }


                                }

              
                                pos=line.find("FUSE_INSTALLED: ");
                                
                                if(pos!=string::npos)
                                {
                                temp=line.substr(pos+16);
                                if(temp=="YES" )
                                {
                                    objectives[OBJ_FUSE_INSTALLED]=true;
                                }
                                else if(temp=="NO" )
                                {
                                    objectives[OBJ_FUSE_INSTALLED]=false;
                                }
                                else 
                                return false;

                                }


                                
                                pos=line.find("LIFT_PASS_COLLECTED: ");
                                if(pos!=string::npos)
                                {
                                temp=line.substr(pos+21);
                                if(temp=="YES" )
                                {
                                    objectives[OBJ_LIFT_PASS]=true;
                                }
                                else if(temp=="NO" )
                                {
                                    objectives[OBJ_LIFT_PASS]=false;
                                }
                                else 
                                return false;
                                }

                            
                                
                                pos=line.find("TRAIN_PREPARED: ");
                    
                                if(pos!=string::npos)
                                {
                                    temp=line.substr(pos+16);
                                if(temp=="YES" )
                                {
                                    objectives[OBJ_TRAIN_PREPARED]=true;
                                }
                                else if(temp=="NO" )
                                {
                                    objectives[OBJ_TRAIN_PREPARED]=false;
                                }

                                else
                                {
                                    return false;
                                }
                                }

                            
                            } 
                            
                            if(!energyFound || !timeFound || !scoreFound || !startRoomFound)
                                return false;
                           
                        
                        

                         return true;


                      }


bool validateStartingRoom(int startFloor, int startRoom)
                          {
         
                            if(startFloor<1 || startFloor>NUM_FLOORS || startRoom<1 || startRoom>ROOMS_PER_FLOOR)
                            return false;

                            return true;

                          }
                

const char* getEntityName(char entitySymbol)
{
    if(entitySymbol=='<')
      return "Master Key ( \"<\" )";

    if(entitySymbol==')')
      return "Energy Pack ( \")\" ) ";

    if(entitySymbol=='\\')
      return "Emergency Toolkit ( \"\\\" )";
    
    if(entitySymbol=='4')
      return "Clue Digit ( \"4\" )";

    if(entitySymbol=='*')
      return "Fuse ( \"*\" )";

    if(entitySymbol=='~')
      return "Cat ( \"~\" )";
    
    if(entitySymbol==';')
      return "Control Panel ( \";\" )";
        
    if(entitySymbol=='(')
      return "Lift Pass ( \"(\" )";
    
    if(entitySymbol=='!')
      return "Passenger ( \"!\" )";
    
    if(entitySymbol=='%')
      return "Medical Kit ( \"%\" )";
    
    if(entitySymbol=='$')
      return "Emergency Access Card ( \"$\" )";

    if(entitySymbol=='2')
      return "Clue Digit ( \"2\" )";
    
    if(entitySymbol=='7')
      return "Clue Digit ( \"7\" )";

    if(entitySymbol=='^')
      return "Cabinet / Train Key ( \"^\" )";

    if(entitySymbol=='@')
      return "Cat Carrier ( \"@\" )";

    if(entitySymbol=='&')
      return "Observation Token ( \"&\" )";


return "Unknown Entity";



}