#include<iostream>
#include "level.h"
#include "util.h"
#include<fstream>
#include<string>
using namespace std;

// Modular implementation using primitive arrays for map parsing and verification

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


                        int posR[ROOMS_PER_FLOOR];
                        bool positionsfound=false;

                        //calculating Floor positions, helps in claculating room positions for entities
                        for(int i=0; i<lines; i++)
                        {
                            int r=0;
                            for(int j=0 ; j<line[i].length(); j++)
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
                            int entityCount=0, r=0;


                            //calculate floor number, helps in storing room connections in the correct floor and room
                             if(line[i].find("F") != string::npos)
                                {
                                int pos=line[i].find("F");
                                 f = line[i][pos + 1] - '0'-1;

                                 //storing the line number in temp
                                 temp=i;
                                  if(f<0 || f>=NUM_FLOORS)
                                    return false;    

                               //k stores the room number

                               int k = 0;

                            //storing room connections in the horizontalConn array
                            for(int j = 0; j < line[i].length(); j++)
                            {
                                if(line[i][j] == 'W' || line[i][j] == 'D' ||
                                    line[i][j] == '#' || line[i][j] == 'L')
                                    {
                                    if(k >= ROOMS_PER_FLOOR - 1)
                                     return false;


                                    
                                     if(line[i][j] == 'L' && j + 1 < line[i].length()
                                      && line[i][j + 1] == '1')
                                     {
                                       // horizontalConn[f][k] = '1'; // Lift 1
                                         j++;
                                      }

                                     else if(line[i][j] == 'L' && j + 1 < line[i].length()
                                      && line[i][j + 1] == '2')
                                     {
                                        // horizontalConn[f][k] = '2'; // Lift 2
                                         j++;
                                      } 
                                    else
                                    {                
                                    horizontalConn[f][k] = line[i][j];
                                    }

                                    k++;
                                    }
                             }

                             if(k != ROOMS_PER_FLOOR - 1)
                                   return false;
                            }
                        //storing entities in the roomEntities array
                         if(i==temp+1)
                         {
                            for(int j=0; j<line[i].length(); j++)
                            {
                             if(r<ROOMS_PER_FLOOR - 1 && posR[r+1]==j)
                                   {
                                    entityCount=0;
                                    r++;}  

                             if(line[i][j]=='<' || line[i][j]==')' || line[i][j]=='\\' || line[i][j]== '4' || line[i][j]=='*' || line[i][j]=='~' || line[i][j]==';' || line[i][j]=='(' || line[i][j]=='^' || line[i][j]=='2' || line[i][j]=='$' || line[i][j]=='%' || line[i][j]=='&' || line[i][j]=='@'|| line[i][j]=='!' || line[i][j]=='7')
                                {
                                     if(entityCount>=MAX_ENTITIES_PER_ROOM)
                                       return false;

                                    entityCount++;
                                    roomEntities[f][r][entityCount-1]=line[i][j];
                                    entityCounts[f][r]=entityCount;

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
                         
                        playerFloor = -1;
                        playerRoom = -1;
                        energy=0;
                        time=0;
                        score=0;
                        inventoryCount=0;
                        emergencyTripsLeft=0;

                        for (int i = 0; i < NUM_OBJECTIVES; i++) {
                            objectives[i] = 0;
                        }
                        
                        for (int i = 0; i < MAX_INVENTORY_CAPACITY; i++) {
                            inventory[i] = '\0';
                        }

                        string line;
                        int pos;
                     
                        while(getline(file, line))
                        {
                                string temp;

                                pos=line.find("PLAYER_FLOOR: ");
                                if(pos!=string::npos)
                                {
                                    temp = line.substr(pos+14);
                                    if(temp!="PLAYER_SELECTS" && temp!="NONE" && temp!="NO")
                                    {
                                        playerFloor = stoi(temp);
                                        if(playerFloor<0 || playerFloor>=NUM_FLOORS)
                                            return false;
                                    }
                                }

                                pos=line.find("PLAYER_ROOM: ");
                                if(pos!=string::npos)
                                {
                                    temp = line.substr(pos+13);
                                    if(temp!="PLAYER_SELECTS" && temp!="NONE" && temp!="NO")
                                    {
                                        playerRoom = stoi(temp);
                                        if(playerRoom<0 || playerRoom>=ROOMS_PER_FLOOR)
                                            return false;
                                    }
                                }

                                pos=line.find("INITIAL_ENERGY: ");
                                if(pos!=string::npos)
                                {
                                     temp = line.substr(pos+16);
                                    if(temp!="PLAYER_SELECTS" && temp!="NONE" && temp!="NO")
                                    {
                                        energy = stoi(temp);
                                        if(energy<0)
                                            return false;
                                    }
                                }

                                pos=line.find("INITIAL_TIME: ");
                                if(pos!=string::npos)
                                {
                                    temp = line.substr(pos+15);
                                    if(temp!="PLAYER_SELECTS" && temp!="NONE" && temp!="NO")
                                    {
                                        time = stoi(temp);
                                        if(time<0)
                                            return false;
                                    }
                                }

                                pos=line.find("INITIAL_SCORE: ");
                                if(pos!=string::npos)
                                {
                                    temp  = line.substr(pos+16);
                                    if(temp!="PLAYER_SELECTS" && temp!="NONE" && temp!="NO")
                                    {
                                        score = stoi(temp);
                                        if(score<0)
                                            return false;
                                    }
                                }

                                pos=line.find("PASSENGER_RESCUED: ");
                                if(pos!=string::npos)
                                {
                                    temp=line.substr(pos+19);
                                    if(temp!="PLAYER_SELECTS" && temp!="NONE" && temp!="NO")
                                     {
                                        if(stoi(temp)<0)
                                            return false;
                                        
                                        objectives[OBJ_PASSENGER_RESCUED] = true;
                                     }
                                }
                                
                                pos=line.find("CAT_RESCUED: ");
                                if(pos!=string::npos)
                                {
                                    temp=line.substr(pos+13);
                                    if(temp!="PLAYER_SELECTS" && temp!="NONE" && temp!="NO")
                                    {
                                        if(stoi(temp)<0)
                                            return false;
                                        
                                        objectives[OBJ_CAT_SECURED] = true;
                                    }
                                }

                                pos=line.find("CLUES_COLLECTED: ");
                                if(pos!=string::npos)
                                {
                                    pos=line.find("4");
                                    if(pos!=string::npos)
                                    {
                                        objectives[OBJ_CLUE_4]=true;
                                    }

                                    pos=line.find("7");
                                    if(pos!=string::npos)
                                    {
                                        objectives[OBJ_CLUE_7]=true;
                                    }

                                    pos=line.find("2");
                                    if(pos!=string::npos)
                                    {
                                        objectives[OBJ_CLUE_2]=true;
                                    }
                                }

                                pos=line.find("CABINET_UNLOCKED: ");
                                temp=line.substr(pos+18);
                                if(temp!="PLAYER_SELECTS" && temp!="NONE" && temp!="NO")
                                {
                                    if(stoi(temp)<0)
                                    return false;

                                    objectives[OBJ_CABINET_UNLOCKED]=true;

                                }


                                pos=line.find("TRAIN_KEY_COLLECTED: ");
                                temp=line.substr(pos+21);
                                if(temp!="PLAYER_SELECTS" && temp!="NONE" && temp!="NO")
                                {
                                    if(stoi(temp)<0)
                                    return false;

                                    objectives[OBJ_TRAIN_KEY]=true;

                                }


                                
                                pos=line.find("FUSE_INSTALLED: ");
                                temp=line.substr(pos+16);
                                if(temp!="PLAYER_SELECTS" && temp!="NONE" && temp!="NO")
                                {
                                    if(stoi(temp)<0)
                                    return false;

                                    objectives[OBJ_FUSE_INSTALLED]=true;

                                }

                                if(objectives[OBJ_FUSE_INSTALLED]==true)
                                {
                                    pos=line.find("FUSE_COLLECTED: ");
                                    temp=line.substr(pos+16);
                                    if(temp!="PLAYER_SELECTS" && temp!="NONE" && temp!="NO")
                                    {
                                    if(stoi(temp)<0)
                                    return false;

                                    objectives[OBJ_FUSE_INSTALLED]=true;

                                    }


                                
                                pos=line.find("LIFT_PASS_COLLECTED: ");
                                temp=line.substr(pos+21);
                                if(temp!="PLAYER_SELECTS" && temp!="NONE" && temp!="NO")
                                {
                                    if(stoi(temp)<0)
                                    return false;

                                    objectives[OBJ_LIFT_PASS]=true;

                                }


                                
                                pos=line.find("TRAIN_PREPARED: ");
                                temp=line.substr(pos+16);
                                if(temp!="PLAYER_SELECTS" && temp!="NONE" && temp!="NO")
                                {
                                    if(stoi(temp)<0)
                                    return false;

                                    objectives[OBJ_TRAIN_PREPARED]=true;

                                }


                                
                          /*      pos=line.find("PLAYER_AT_TRAIN: ");
                                temp=line.substr(pos+17);
                                if(temp!="PLAYER_SELECTS" && temp!="NONE" && temp!="NO")
                                {
                                    if(stoi(temp)<0)
                                    return false;

                                    objectives[OBJ_CABINET_UNLOCKED]=true;

                                }*/

                            
                            }
 
                            return true;      

                              
                        
                        }



                      }


bool validateStartingRoom(int startFloor, int startRoom,
                          const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1])
                          {
                            //initialize right and left connections to empty
                            char connLeft=' ', connRight=' ';
                              
                            //reject out of bound starting floors and starting rooms
                            if(startFloor<1 || startFloor>NUM_FLOORS || startRoom<1 || startRoom>ROOMS_PER_FLOOR)
                            return false;

                             
                            //vertical conditions, handled based on assumptions!
                           //lifts not handled ?


                           





                            //at room 1 in all floors, there is vertical connection through stairs  
                            if(startRoom==1)
                                return true;

                            //left connections, not possible for room1
                            if(startRoom>1)
                            connLeft=horizontalConn[startFloor-1][startRoom-2];
                            
                            //right connections, not possible for room5
                            if(startRoom<ROOMS_PER_FLOOR)
                            connRight=horizontalConn[startFloor-1][startRoom-1];
                           
                            //check wether connections exist
                           if((connRight==' ' || connRight=='#') && ( connLeft==' ' || connLeft=='#'))
                           return false;

                            return true;

                          }
                

const char* getEntityName(char entitySymbol)
{
    if(entitySymbol=='<')
      return "Master Key";

    if(entitySymbol==')')
      return "Energy Packs";

    if(entitySymbol=='\\')
      return "Emergency Toolkit";
    
    if(entitySymbol=='4')
      return "Clue Digit 4";

    if(entitySymbol=='*')
      return "Fuse";

    if(entitySymbol=='~')
      return "Cat";
    
    if(entitySymbol==';')
      return "Control Panel";
        
    if(entitySymbol=='(')
      return "Lift Pass";
    
    if(entitySymbol=='!')
      return "Passenger";
    
    if(entitySymbol=='%')
      return "Medical Kit";
    
    if(entitySymbol=='$')
      return "Emergency Access Card";

    if(entitySymbol=='2')
      return "Clue Digit 2";
    
    if(entitySymbol=='7')
      return "Clue Digit 7";

    if(entitySymbol=='^')
      return "Cabinet / Train Key";

    if(entitySymbol=='@')
      return "Cat Carrier";

    if(entitySymbol=='&')
      return "Observation Token";



}