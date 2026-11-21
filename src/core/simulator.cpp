#include <iostream>
#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
#include "level.h"
using namespace std;


// Modular simulation loop using primitive variables
void runSimulation(int& playerFloor, int& playerRoom,
                   int& energy, int& time, int& score,
                   char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                   bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                   char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                   char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                   int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
                   {
                    //declarre varaibles
                    char command;
                    int targetRoom, targetFloor;
                   
                    // set gamerunning true at the start of the game loop
                    bool gameRunning = true;

                    //main loop
                    while(gameRunning)
                    {
                        //display current status
                        cout<<"--------------------Current Status--------------------"<<endl;

                        cout<<"\nLocation: Floor "<<playerFloor<<", Room "<<playerRoom<<endl;

                        cout<<"\nEnergy Remeaining: "<<energy<<endl;
                        cout<<"Time Remaining: "<<time<<endl;
                        cout<<"Score: "<<score<<endl;
                        cout<<"Emergency Trips Left: "<<emergencyTripsLeft<<endl;

                        //display inventory
                        cout<<"\n Inventory: ";
                       
                        //if inventory count is 0 display empty
                        if(inventoryCount==0)
                        cout<<"empty"<<endl;
                        
                        //convert the inventory symbols to their names and display
                        else
                        {
                        for(int i=0; i<inventoryCount; i++)
                            cout<<getEntityName(inventory[i])<<" ";
                        
                        cout<<endl;
                         }

                        //display objectives
                        cout<<"\nPassenger: "<<(objectives[OBJ_PASSENGER_RESCUED]? "Rescued" : "Not Rescued")<<endl;
                        cout<<"Cat: "<<(objectives[OBJ_CAT_SECURED]? "Secured" : "Not Secured")<<endl;
                        cout<<"Cabinet: "<<(objectives[OBJ_CABINET_UNLOCKED]? "Unlocked" : "Locked")<<endl;
                        cout<<"Train key: "<<(objectives[OBJ_TRAIN_KEY]? "Collected" : "Not Collected")<<endl;
                        cout<<"Train Prepared: "<<(objectives[OBJ_TRAIN_PREPARED]? "yes" : "No")<<endl;
                        cout<<"Fuse Collected: "<<(objectives[OBJ_FUSE_COLLECTED]? "Yes" : "No")<<endl;
                        cout<<"Fuse Installed: "<<(objectives[OBJ_FUSE_INSTALLED]? "Yes" : "No")<<endl;
                        cout<<"Lift pass: "<<(objectives[OBJ_LIFT_PASS]? "Collected" : "Not Collected")<<endl;
                        cout<<"Lift 2: "<<(objectives[OBJ_LIFT2_ACTIVE]? "operational" : "Not operational")<<endl;

                         //take input from the user for next command;
                        cout<<"\nEnter command (M=move, I=inspect, C=collect an item, U=use, X=unlock cabinet, H=help, Q=quit): ";
                        cin>>command;

                        //convert the lowercase input commands to uppercase for coneveninece
                        if(command>='a' && command<='z')
                            command=(command-97)+65;


                         // handles invlaid inputs
                         if(command!='M' && command!='I' && command!='C' && command!='U' && command!='X' && command!='H' && command!='Q')
                        {
                            cout<<"Invalid Command!\n"<<endl;
                            continue;


                        }


                         char symbol;

                        //carry out commands
                        switch(command)
                        {
                            //handle movemment command
                            case 'M':
                            cout<<"Enter the target Floor and Room: ";
                            cin>>targetFloor>>targetRoom;
                           
                            //call the handle movemnet function
                            handleMoveCommand(targetFloor, targetRoom, playerFloor, playerRoom, energy, time, horizontalConn, inventory, inventoryCount, emergencyTripsLeft, objectives[OBJ_LIFT2_ACTIVE]);

                            break;

                            //hndle inspection command
                            case 'I':
                            //call the fucntion to handle inspection command
                            handleInspectCommand(playerFloor, playerRoom,
                            energy, time, roomEntities,entityCounts[playerFloor-1][playerRoom-1], objectives,score);
                            break;


                            //handle collection command
                            case 'C':
                            //user choses the item to collect;
                            cout<<"Enter the item symbol to collect: ";
                            cin>>symbol;  

                            //call the funtion to handle collection command
                            handleCollectCommand(symbol, playerFloor, playerRoom, energy, time, score, roomEntities, entityCounts[playerFloor-1][playerRoom-1], inventory, inventoryCount, objectives);
                            break;


                            //handle use command
                            case 'U':
                            //user choses the item to use;
                            cout<<"Enter the item symbol to use: ";
                            cin>>symbol;

                            //call the function to handle use command
                            handleUseCommand(symbol, playerFloor, playerRoom, energy, time, score, inventory, inventoryCount, objectives);
                            break;


                            //handle unlock cabinet coommand
                            case 'X':
                            {
                            //user enters the code to unlock the cabinet
                            string code;
                            cout<<"Enter the 3 digit code to unlock the cabinet: ";
                            cin>>code;


                            //validate the code
                            if(code.length()!=3 || code[0]<'0' || code[0]> '9' || code[1]<'0' || code[1]>'9' || code[2]<'0' || code[2]>'9')
                            {
                                cout<<"Invalid code! Code must be 3 digits."<<endl;
                            }
                            //code is enterd using string,
                            //we pass the the memory of first index of string code in the function
                            
                            //call the function to handle unlock cabinet command
                           else
                            {
                                handleUnlockCabinetCommand(&code[0], playerFloor, playerRoom, objectives, score, roomEntities,entityCounts[playerFloor-1][playerRoom-1]);                            
                            }

                            break;
                            }
                            

                            case 'H':
                            //disply help menu;
                            cout<<"\nHelp Menu:"<<endl;
                            cout<<"M: Move to a different room (requires floor and room number)"<<endl;
                            cout<<"I: Inspect the current room for items and entities"<<endl;
                            cout<<"C: Collect an item from the current room"<<endl;
                            cout<<"U: Use an item from your inventory"<<endl;
                            cout<<"X: Unlock a cabinet using a code"<<endl;
                            cout<<"H: Show this help menu"<<endl;
                            cout<<"Q: Quit the game"<<endl;
                            break;

                            //quits the game;
                            case 'Q':
                            gameRunning=false;
                            break;

                        }

                        if(command!='Q' && !checkEndConditions(energy, time, objectives))
                            gameRunning=false;

                        }



                   }

                






bool handleMoveCommand(int targetFloor, int targetRoom,
                       int& currentFloor, int& currentRoom,
                       int& energy, int& time,
                       const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                       const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                       int& emergencyTripsLeft, bool lift2Operational)
                       {

                        //check wether target floor and room or valid
                        if(targetFloor<1 || targetFloor>NUM_FLOORS || targetRoom<1 || targetRoom>ROOMS_PER_FLOOR)
                        {
                            cout<<"Invalid target floor or room!"<<endl;
                            return false;
                        }

                        if(targetFloor==currentFloor && targetRoom==currentRoom)
                        {
                            cout<<"You are already in the target room!"<<endl;
                            return false;
                        }

                        char ConnType=' ';

                        //call isDirectlyConnected function to check wthwer a connection exists, pass ConnType(by refrence)
                        //connType would store the connection type, empty" " for none 
                        if(!isDirectlyConnected(currentFloor, currentRoom, targetFloor, targetRoom, horizontalConn, ConnType))
                           {
                            cout<<"No direct connection, Movement Failed!"<<endl;
                            return false;
                           }


                        //check wether there are not enough resources for the movement
                        int timeDelta=0, energyDelta=0;

                        if(ConnType=='D' || ConnType=='S')
                        {
                            energyDelta=-2;
                            timeDelta=-1;
                        }

                        else if(ConnType=='W')
                        {
                            energyDelta=-3;
                            timeDelta=-2;
                        }

                        else if(ConnType=='1' || ConnType=='2')
                        {
                            energyDelta=-4;
                            timeDelta=-3;
                        }
                        
                        //if there is not enough resources, movement is failed 
                        if(energy+energyDelta<0 || time+timeDelta<0)
                        {
                            cout<<"Not enough Resources, Movement Failed!"<<endl;
                            return false;
                        }


                        //then check despit the connection, does the user has tools to traverse
                        if(!canTraverse(targetFloor, targetRoom, ConnType, inventory, inventoryCount, emergencyTripsLeft, lift2Operational))
                        {
                            cout<<"path not traversible, Movement Failed!"<<endl;
                            return false;
                        }

                        
                        //call the applyResourceDelta function to update resources
                        applyResourceDelta(energy, time, energyDelta, timeDelta);

                          
                      //after all the processes, update the room and floor
                      currentFloor=targetFloor;
                      currentRoom=targetRoom;

                      return true;

                }





bool handleInspectCommand(int currentFloor, int currentRoom,
                          int& energy, int& time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES], int& score)
                          {

                          }





bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,
                          int& energy, int& time, int& score,
                          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int& entityCount,
                          char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                          bool objectives[NUM_OBJECTIVES])
                          {}





bool handleUseCommand(char itemSymbol, int currentFloor, int currentRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES])
                      {}





bool handleUnlockCabinetCommand(const char* code, int currentFloor, int currentRoom,
                                bool objectives[NUM_OBJECTIVES], int& score,
                                char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                                int& entityCount)
                                {}





bool checkEndConditions(int energy, int time, const bool objectives[NUM_OBJECTIVES])
{
    if(energy==0 || time==0)
    return false;
}