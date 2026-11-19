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

                        
                        char con;
                        int temp=-1;


                        // initializing entity counts to zero
                        for(int i=0; i<NUM_FLOORS; i++)
                        {
                            for(int j=0; j<ROOMS_PER_FLOOR; j++)
                            {
                                entityCounts[i][j]=0;
                            }   
                        }
                         

                        int posR[ROOMS_PER_FLOOR];

                        //calculating Floor positions, helps in claculating room positions for entities
                        for(int i=0; i<lines; i++)
                        {
                            bool found=false;
                            for(int j=0, r=0; j<line[i].length(); j++)
                             {
                              if(line[i][j]=='F')
                              {
                                 posR[r]=j;
                                  r++;
                                  found=true;
                               }
 
                            }

                            if(found)
                            break;
                        }
                         int f=-1;

                       //read linearray character by charater
                       for(int i=0; i<lines; i++)
                        {
                            int entityCount=0, r=0;
                            bool found=false;
                        
                              

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
                                    horizontalConn[f][k] = '1'; // Lift 1
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

                             if(line[i][j]=='<' || line[i][j]==')' || line[i][j]=='\' || line[i][j]=='4' || line[i][j]=='*' || line[i][j]=='~' || line[i][j]==';' || line[i][j]=='(' || line[i][j]=='^' || line[i][j]=='2' || line[i][j]=='$' || line[i][j]=='%' || line[i][j]=='&' || line[i][j]=='@''|| line[i][j]=='!' || line[i][j]=='7')
                                {
                                     if(entityCount>=MAX_ENTITIES_PER_ROOM)
                                       return false;

                                    entityCount++;
                                    roomEntities[f][r][entityCount-1]=line[i][j];
                                    entityCounts[f][r]=entityCount;


                                    found=true;
                                }

                                 
                            }
                       }
                 }
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
                         
                        







                      }


bool validateStartingRoom(int startFloor, int startRoom,
                          const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1])
                          {


                          }
                

const char* getEntityName(char entitySymbol)
{


}