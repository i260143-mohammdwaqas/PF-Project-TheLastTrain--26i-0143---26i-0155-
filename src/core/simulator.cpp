#include <iostream>
#include "physics.h"
#include "simulator.h"
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


                        //check wether there are Resources not enough, collection for the movement
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
                        
                        //if there is Resources not enough, collection, movement is failed 
                        if(energy+energyDelta<0 || time+timeDelta<0)
                        {
                            cout<<"Resources not enough, Movement Failed!"<<endl;
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




//objectives and score parameters left unattended for now;
bool handleInspectCommand(int currentFloor, int currentRoom,
                          int& energy, int& time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES], int& score)
                          { 
                            int energyDelta=-1, timeDelta=-1;

                            //check if currentFloor and currentRoom are valid
                            if(currentRoom<1 || currentRoom>ROOMS_PER_FLOOR || currentFloor<1 || currentFloor>NUM_FLOORS)
                            return 0;
                            
                            //check if the player has enough time nd energy for inspection command, if not return false
                            if(energy+energyDelta<0 || time+timeDelta<0)
                            {
                                cout<<"Resources not enough, Inspection Failed!"<<endl;
                                return false;
                            }
 
                             int deltaScore=0;

                            //print room entites
                            if(entityCount==0)
                            cout<<"current Room does not contain any items"<<endl;
                            
                           

                            else
                            {
                            cout<<"Romm entities: ";
                            for(int i=0; i<entityCount; i++)
                            {
                            cout<<getEntityName(roomEntities[currentFloor-1][currentRoom-1][i])<<" ";

                            //clues are not exposed on inspection
                            //update the score on inspection of clues, if they are present in the room

                            if(roomEntities[currentFloor-1][currentRoom-1][i]=='4' && !objectives[OBJ_CLUE_4])
                            {
                                objectives[OBJ_CLUE_4]=true;
                                deltaScore=10;
                            }

                            if(roomEntities[currentFloor-1][currentRoom-1][i]=='2'  && !objectives[OBJ_CLUE_2])
                            {
                                objectives[OBJ_CLUE_2]=true;
                                deltaScore=10;
                            }

                            if(roomEntities[currentFloor-1][currentRoom-1][i]=='7' && !objectives[OBJ_CLUE_7])
                            {
                                objectives[OBJ_CLUE_7]=true;
                                deltaScore=10;
                            }
                        }
                    }
                             
                            //call the function to update the score, for clues;
                            applyScoreDelta(score, deltaScore);
                          
                            //update resources, used for inspection
                            applyResourceDelta(energy, time, energyDelta, timeDelta);
                            

                            return true;

                }
            




bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,
                          int& energy, int& time, int& score,
                          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int& entityCount,
                          char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                          bool objectives[NUM_OBJECTIVES])
                          {

                            if(currentFloor<1 || currentFloor>NUM_FLOORS || currentRoom<1 || currentRoom>ROOMS_PER_FLOOR)
                            return false;


                            //if itemSymbol is clue reject collection
                            if(itemSymbol=='4' || itemSymbol=='2' || itemSymbol=='7')
                            {
                                cout<<"Clue items cannot be collected!"<<endl;
                                return false;
                            }


                            int energyDelta=-1, timeDelta=-1;

                            //speacial energy and time restorartion conditions for certain symbols
                            //these items have immediate effects on collection, they dont get stored in the inventory
                            if(itemSymbol==ENT_ENERGY_PACK)
                            energyDelta=25;

                            if(itemSymbol==ENT_EMERGENCY_TOOLKIT)
                            energyDelta=15;

                            if(itemSymbol==ENT_OBSERVATION_TOKEN)
                            timeDelta=15;
                        

                            //if energy and time are not enough to carry out the command, return false;
                            if(energy+energyDelta<0 || time+timeDelta<0)
                            {
                                cout<<"Resources not enough, item collection Failed!"<<endl;
                                return false;
                            }

                              
                            //if item to be collected is not in the room, return false;
                            bool found=false;
                            int itemIndex=-1;
                           for(int i=0; i<entityCount; i++)
                            {
                                if(roomEntities[currentFloor-1][currentRoom-1][i]==itemSymbol)
                                {
                                    found=true;
                                    itemIndex=i;
                                    break;
                                }
                            }
                           
                            if(!found)
                            {
                                cout<<"Item in not present, Collection failed"<<endl;
                                return false;
                            }
                             

                            
                            //if the item is not one of the three, then its added to the inventory
                           if(!(itemSymbol==ENT_ENERGY_PACK || itemSymbol==ENT_EMERGENCY_TOOLKIT || itemSymbol==ENT_OBSERVATION_TOKEN)) 
                           {
                            //call addItem function, first check wether fucntion runs succesfully, if not retiurn false
                            if(!addItem(inventory, inventoryCount, itemSymbol))
                            return false;
                           }
                           
                           
                            //remove the item from the entity 
                            for(int i=itemIndex; i<entityCount-1; i++)
                            {
                                roomEntities[currentFloor-1][currentRoom-1][i]= roomEntities[currentFloor-1][currentRoom-1][i+1];
                            }
                           
                            //adjust the entityCount
                            entityCount--;


                            //update score;
                            int deltaScore=0;
                            if(itemSymbol=='<' )
                            deltaScore=10;
                          
                            //call the function to update the score;
                            applyScoreDelta(score, deltaScore);

                            //update resources
                            applyResourceDelta(energy, time, energyDelta, timeDelta);

                            //update objective which can become true, when items are collected
                            if(itemSymbol==ENT_MASTER_KEY)
                                objectives[OBJ_MASTER_KEY] = true;

                            if(itemSymbol==ENT_FUSE)
                                objectives[OBJ_FUSE_COLLECTED] = true;

                            if(itemSymbol==ENT_LIFT_PASS)
                                objectives[OBJ_LIFT_PASS] = true;

                            if(itemSymbol==ENT_CAT_CARRIER)
                                objectives[OBJ_CAT_CARRIER] = true;
                            
                             
                            return true;                                    
                          }





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