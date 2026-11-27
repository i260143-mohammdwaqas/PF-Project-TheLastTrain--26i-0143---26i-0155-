#include <iostream>
#include "physics.h"
#include "simulator.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
#include "level.h"
using namespace std;


//handleUnlockCabinet? (not sure about wether some items should be remved, once used), to be handled latter?
//confirm which item shoud be handled by handleusecommand?
//changed the parameters of checkEndConditions??

// Modular simulation loop using primitive variables
void runSimulation(int& playerFloor, int& playerRoom,
                   int& energy, int& time, int& score,
                   char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                   bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                   char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                   char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                   int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
                   {

                    //validate the starting room, and starting floor
                    if(!validateStartingRoom(playerFloor, playerRoom))
                    {
                        cout<<"Invalid starting room or floor!"<<endl;
                        return;
                    }

                    //declarre varaibles
                    char command;
                    int targetRoom, targetFloor;
                   
                    // set gamerunning true at the start of the game loop
                    bool gameRunning=true;

                    //main loop
                    while(gameRunning)
                    {
                        clearScreen();
                        printHeader("THE LAST TRAIN");
                        //display current status
                        //cout<<"--------------------Current Status--------------------"<<endl;

                        //display inventory, score, energy, time, location, emergency trips left
                        renderDashboard(playerFloor, playerRoom, energy, time, score, inventory, inventoryCount, emergencyTripsLeft);

                        //display objectives checklist
                        renderObjectivesChecklist(objectives);

                        //dislay the building in ascii art, with the player location marked
                        renderAsciiBuilding(playerFloor, playerRoom);

                         //take input from the user for next command;
                        cout<<"\nEnter command (M=move, I=inspect, C=collect an item, U=use, X=unlock cabinet, S=save, H=help, Q=quit): ";
                        
                        if(!(cin>>command))
                        {
                            cout<<"Invalid input for command!"<<endl;
                            cin.clear();
                            cin.ignore(1000, '\n');

                            cout << "\nPress Enter to continue...";
                            cin.get();

                            continue;
                        }

                        //convert the lowercase input commands to uppercase for coneveninece
                        if(command>='a' && command<='z')
                            command=(command-97)+65;


                         // handles invlaid inputs
                         if(command!='M' && command!='I' && command!='C' && command!='U' && command!='X' && command!='S' && command!='H' && command!='Q')
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
                         

                            if(!(cin>>targetFloor>>targetRoom))
                            {
                                cout<<"Invalid input for floor and room!"<<endl;
                                cin.clear();
                                cin.ignore(1000, '\n');

                                cout << "\nPress Enter to continue...";
                                cin.get();

                                break;
                            }
                          

                            //call the handle movemnet function
                            handleMoveCommand(targetFloor, targetRoom, playerFloor, playerRoom, energy, time, horizontalConn, inventory, inventoryCount, emergencyTripsLeft, objectives[OBJ_LIFT2_ACTIVE]);

                            //add a pause after inspection, so that the user can read the output before the screen is cleared
                            cout << "\nPress Enter to continue...";
                            cin.ignore(1000, '\n');
                            cin.get();

                            break;

                            //hndle inspection command
                            case 'I':
                            //call the fucntion to handle inspection command
                            handleInspectCommand(playerFloor, playerRoom,
                            energy, time, roomEntities,entityCounts[playerFloor-1][playerRoom-1], objectives,score);

                            //add a pause after inspection, so that the user can read the output before the screen is cleared
                            cout << "\nPress Enter to continue...";
                            cin.ignore(1000, '\n');
                            cin.get();

                            break;


                            //handle collection command
                            case 'C':
                            //user choses the item to collect;
                            cout<<"Enter the item symbol to collect: ";
                          

                            if(!(cin>>symbol))
                            {
                                cout<<"Invalid input for item symbol!"<<endl;
                                cin.clear();
                                cin.ignore(1000, '\n');

                                cout << "\nPress Enter to continue...";
                                cin.get();

                                break;
                            }

                            //call the funtion to handle collection command
                            handleCollectCommand(symbol, playerFloor, playerRoom, energy, time, score, emergencyTripsLeft, roomEntities, entityCounts[playerFloor-1][playerRoom-1], inventory, inventoryCount, objectives);
                           
                            cout << "\nPress Enter to continue...";
                            cin.ignore(1000, '\n');
                            cin.get();

                            break;


                            //handle use command
                            case 'U':
                            //user choses the item to use;
                            cout<<"Enter the item symbol to use: ";
                            
                            if(!(cin>>symbol))
                            {
                                cout<<"Invalid input for item symbol!"<<endl;
                                cin.clear();

                                cin.ignore(1000, '\n');
                                cout << "\nPress Enter to continue...";
                                cin.get();

                                break;
                            }



                            //call the function to handle use command
                            handleUseCommand(symbol, playerFloor, playerRoom, energy, time, score, inventory, inventoryCount, objectives);
                            
                            cout << "\nPress Enter to continue...";
                            cin.ignore(1000, '\n');
                            cin.get();

                            break;


                            //handle unlock cabinet coommand
                            case 'X':
                            {
                            //check if the cabinet is alredy unlocked
                            if(objectives[OBJ_CABINET_UNLOCKED])
                            {
                                cout<<"Cabinet is alredy unlocked!"<<endl;
                                break;
                            }

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
                                handleUnlockCabinetCommand(&code[0], playerFloor, playerRoom, objectives, score);                            
                            }
                            
                            cout << "\nPress Enter to continue...";
                            cin.ignore(1000, '\n');
                            cin.get();

                            break;
                            }

                            case 'S':
                            {

                                //call the function to save the game state to file
                                if(saveGameStateToFile(SAVE_FILE_NAME, playerFloor, playerRoom, energy, time, score, inventory, inventoryCount, objectives, emergencyTripsLeft, roomEntities, entityCounts))
                                {
                                    cout<<"Game state saved successfully!"<<endl;
                                }
                                else
                                {
                                    cout<<"Failed to save game state!"<<endl;
                                }

                                //to ensure that clea screen function does not remove the message before the user can read it, we ask the user to press enter to continue
                                    cout << "Press Enter to continue...";
                                    cin.ignore(1000, '\n');
                                    cin.get();

                                break;
                            }
                            

                            case 'H':
                            //disply help menu;
                            cout<<"\nHelp Menu:"<<endl;
                            cout<<"M: Move to a different room (requires floor and room number)"<<endl;
                            cout<<"I: Inspect the current room for items and entities"<<endl;
                            cout<<"C: Collect an item from the current room"<<endl;
                            cout<<"U: Use an item from Playerr inventory"<<endl;
                            cout<<"X: Unlock a cabinet using a code"<<endl;
                            cout<<"S: Save the current game state"<<endl;
                            cout<<"H: Show this help menu"<<endl;
                            cout<<"Q: Quit the game"<<endl;

                            //add a pause
                            cout << "\nPress Enter to continue...";
                            cin.ignore(1000, '\n');
                            cin.get();

                            break;

                            //quits the game;
                            case 'Q':
                            gameRunning=false;
                            break;

                            default:
                            cout<<"Invalid Command!"<<endl;

                            //add a puase
                            cout << "\nPress Enter to continue...";
                            cin.ignore(1000, '\n');
                            cin.get();
                            break;


                        }


                        if(command!='Q' && checkEndConditions(energy, time, score, objectives))
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
                        if(!validateStartingRoom(targetFloor, targetRoom))
                        {
                            cout<<"Invalid target floor or room!"<<endl;
                            return false;
                        }
                        
                   

                        if(targetFloor==currentFloor && targetRoom==currentRoom)
                        {
                            cout<<"Player are already in the target room!"<<endl;
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
                            energyDelta=COST_MOVE_ORDINARY_ENERGY;
                            timeDelta=COST_MOVE_ORDINARY_TIME;
                        }

                        else if(ConnType=='W')
                        {
                            energyDelta=COST_MOVE_WINDOW_ENERGY;
                            timeDelta=COST_MOVE_WINDOW_TIME;
                        }

                        else if(ConnType=='1' || ConnType=='2')
                        {
                            energyDelta=COST_MOVE_LIFT_ENERGY;
                            timeDelta=COST_MOVE_LIFT_TIME;
                        }
                        
                        //if there is Resources not enough, collection, movement is failed 
                        if(energy+energyDelta<0 || time+timeDelta<0)
                        {
                            cout<<"Resources not enough, Movement Failed!"<<endl;
                            return false;
                        }


                        //then check despit the connection, does the user has tools to traverse
                        if(!canTraverse(ConnType, inventory, inventoryCount, emergencyTripsLeft, lift2Operational))
                        {
                            cout<<"path not traversible, Movement Failed!"<<endl;
                            return false;
                        }

                        //call the applyResourceDelta function to update resources
                        applyResourceDelta(energy, time, energyDelta, timeDelta);

                        
                        //decrement emergency trips left if lift2 is used and it is not operational
                        if(ConnType=='2' && !lift2Operational)
                        {
                            emergencyTripsLeft--;
                        }
                        
        
                          
                      //after all the processes, update the room and floor
                      currentFloor=targetFloor;
                      currentRoom=targetRoom;


                      cout<<"Player moved to Floor "<<currentFloor<<", Room "<<currentRoom<<" successfully!"<<endl;
                      return true;

                }




//objectives and score parameters left unattended for now;
bool handleInspectCommand(int currentFloor, int currentRoom,
                          int& energy, int& time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES], int& score)
                          { 
                            int energyDelta=COST_INSPECT_ENERGY, timeDelta=COST_INSPECT_TIME;

                                    //validate
                            if(entityCount <0 || entityCount > MAX_ENTITIES_PER_ROOM)
                            {
                               cout<<"Invalid entity count!"<<endl;
                               return false;
                             }
                                                  
                            //check if the player has enough time nd energy for inspection command, if not return false
                            if(energy+energyDelta<0 || time+timeDelta<0)
                            {
                                cout<<"Resources not enough, Inspection Failed!"<<endl;
                                return false;
                            }
 
                             int deltaScore=0;

                            //print room entites
                            renderRoomDetails(currentFloor, currentRoom, roomEntities, entityCount);
                            
                        
                            for(int i=0; i<entityCount; i++)
                            {
                            //clues are exposed on inspection
                            //update the score on inspection of clues, if they are present in the room

                            if(roomEntities[currentFloor-1][currentRoom-1][i]=='4' && !objectives[OBJ_CLUE_4])
                            {
                                objectives[OBJ_CLUE_4]=true;
                                deltaScore+=SCORE_CLUE_FOUND;
                            }

                            if(roomEntities[currentFloor-1][currentRoom-1][i]=='2'  && !objectives[OBJ_CLUE_2])
                            {
                                objectives[OBJ_CLUE_2]=true;
                                deltaScore+=SCORE_CLUE_FOUND;
                            }

                            if(roomEntities[currentFloor-1][currentRoom-1][i]=='7' && !objectives[OBJ_CLUE_7])
                            {
                                objectives[OBJ_CLUE_7]=true;
                                deltaScore+=SCORE_CLUE_FOUND;
                            }
                          
                            }
                    
                             
                            //call the function to update the score, for clues;
                            applyScoreDelta(score, deltaScore);
                          
                            //update resources, used for inspection
                            applyResourceDelta(energy, time, energyDelta, timeDelta);
                            

                            return true;

                }
            




bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,
                          int& energy, int& time, int& score,int& emergencyTripsLeft,
                          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int& entityCount,
                          char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                          bool objectives[NUM_OBJECTIVES])
                          {
                            //validate
                            if(entityCount <0 || entityCount > MAX_ENTITIES_PER_ROOM)
                            {
                               cout<<"Invalid entity count!"<<endl;
                               return false;
                             }

                            if(inventoryCount<0 || inventoryCount>MAX_INVENTORY_CAPACITY)
                            {
                               cout<<"Invalid inventory count!"<<endl;
                               return false;
                            }

                            //if itemSymbol is clue reject collection
                            if(itemSymbol==ENT_CLUE_4 || itemSymbol==ENT_CLUE_2 || itemSymbol==ENT_CLUE_7)
                            {
                                cout<<"Clue items cannot be collected!"<<endl;
                                return false;
                            }


                            int energyDelta=COST_ITEM_PICKUP_ENERGY, timeDelta=COST_ITEM_PICKUP_TIME;

                            //speacial energy and time restorartion conditions for certain symbols
                            //these items have immediate effects on collection, they dont get stored in the inventory
                            if(itemSymbol==ENT_ENERGY_PACK)
                            {
                                energyDelta=BONUS_ENERGY_PACK;
                                timeDelta=0;
                            }


                            if(itemSymbol==ENT_EMERGENCY_TOOLKIT)
                            {
                                energyDelta=BONUS_EMERGENCY_TOOLKIT;
                                timeDelta=0;
                            }

                            if(itemSymbol==ENT_OBSERVATION_TOKEN)
                            {
                                timeDelta=BONUS_OBSERVATION_TOKEN;
                                energyDelta=0;
                            }
                        

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
                             
                            if(itemSymbol==ENT_CABINET_TRAIN_KEY && !objectives[OBJ_CABINET_UNLOCKED])
                            {
                               cout<<"cabinet is Locked!"<<endl;
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

                            if(itemSymbol==ENT_MASTER_KEY )
                            deltaScore=SCORE_KEY_COLLECT;

                            if(itemSymbol==ENT_CABINET_TRAIN_KEY)
                            deltaScore=SCORE_TRAIN_KEY;
                            
                            if(itemSymbol==ENT_LIFT_PASS)
                            deltaScore=SCORE_LIFT_PASS;

                           
                            //call the function to update the score;
                            applyScoreDelta(score, deltaScore);

                            //update resources
                            applyResourceDelta(energy, time, energyDelta, timeDelta);

                            //update objective which can become true, when items are collected
                            if(itemSymbol==ENT_MASTER_KEY)
                                objectives[OBJ_MASTER_KEY]=true;

                            if(itemSymbol==ENT_EMERGENCY_CARD)
                                emergencyTripsLeft=3;

                            if(itemSymbol==ENT_FUSE)
                                objectives[OBJ_FUSE_COLLECTED]=true;

                            if(itemSymbol==ENT_LIFT_PASS)
                               {
                                objectives[OBJ_LIFT_PASS]=true;
                                //if fuse is already installed, then lift2 becomes operational automatically
                                if(objectives[OBJ_FUSE_INSTALLED])
                                objectives[OBJ_LIFT2_ACTIVE]=true;
                               }

                            if(itemSymbol==ENT_CABINET_TRAIN_KEY)
                               objectives[OBJ_TRAIN_KEY]=true;


                            if(itemSymbol==ENT_CAT_CARRIER)
                                objectives[OBJ_CAT_CARRIER]=true;
                            
                             
                            return true;                                    
                          }





bool handleUseCommand(char itemSymbol, int currentFloor, int currentRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES])
                      {
                        //validate
                        if(inventoryCount < 0 || inventoryCount > MAX_INVENTORY_CAPACITY)
                        {
                            cout << "Invalid inventory count!" << endl;
                            return false;
                       }
                        
                        //check if item can be used here
                        if(itemSymbol!=ENT_FUSE && itemSymbol!=ENT_MEDICAL_KIT && itemSymbol!=ENT_CAT_CARRIER && itemSymbol!=ENT_CABINET_TRAIN_KEY)
                        {
                            cout<<"Item cannot be used here!"<<endl;
                            return false;
                        }

                        //check wether the item exists in the inventory, 
                        bool found=false;
                        for(int i=0; i<inventoryCount; i++)
                        {
                            if(inventory[i]==itemSymbol)
                            {
                                found=true;
                                break;
                            }

                        }



                        //if not return false
                        if(!found)
                        {
                            cout<<"Player does not have the item!"<<endl;
                            return false;
                        }

                        //declare/initialize the delta variables
                        int deltaEnergy=COST_ITEM_USE_ENERGY, deltaTime=COST_ITEM_USE_TIME, deltaScore=0;
                        
                        //check wether user has enough resources to use the item;
                        if(energy+deltaEnergy<0 || time+deltaTime<0)
                        {
                            cout<<"Player does not have enough resources!"<<endl;
                            return false;
                        }

                        //if item is fuse
                        if(itemSymbol==ENT_FUSE)
                        {
                            //check if plyer is at required location
                            if(currentFloor!=3 || currentRoom!=4)
                            {
                                cout<<"Player must be at Room 4 in 3rd Floor!"<<endl;
                                return false;
                            }

                            //call the function, it handles the fuse installation, and updates the score and objectives
                            if(!installFuseControlPanel(inventory, inventoryCount, objectives, score))
                            {
                                cout<<"Fuse installation failed!"<<endl;
                                return false;
                            }

                            //check if OBJ_LIFT_PASS is true, if yes then OBJ_LIFT2_ACTIVE become true automatically
                            if(objectives[OBJ_LIFT_PASS])
                            objectives[OBJ_LIFT2_ACTIVE]=true;
                          
                        }


                        //when item symbol is medical ki
                        if(itemSymbol==ENT_MEDICAL_KIT)
                        {
                            //check if the player is at required location
                            if(currentFloor!=4 || currentRoom!=1)
                            {
                                cout<<"Player must be at Room 1 in 4th Floor!"<<endl;
                                return false;
                            }

                            //check if the passenger has alredy been rescued
                            if(objectives[OBJ_PASSENGER_RESCUED])
                            {
                                cout<<"Passenger has already been rescued!"<<endl;
                                return false;
                            }

                            //remove the medical kit from the inventory
                             if(!removeItem(inventory, inventoryCount, itemSymbol))
                           {
                                cout << "Failed to remove item from inventory!" << endl;
                                return false;
                            } 

                            //set the OBJ_PASSENGER_RESCUED true
                            objectives[OBJ_PASSENGER_RESCUED]=true;

                            //update the delta score value
                            deltaScore=SCORE_RESCUE_PASSENGER;
                        }

                        
                        
                        //if item symbol is cat carrier;
                        if(itemSymbol==ENT_CAT_CARRIER)
                        {
                            //check wether player is at required location
                            if(currentFloor!=3 || currentRoom!=3)
                            {
                                cout<<"Player must be at Room 3 in 3rd Floor!"<<endl;
                                return false;
                            }

                            //check if objectives[OBJ_CAT_SECURED] is already true
                            if(objectives[OBJ_CAT_SECURED])
                            {
                                cout<<"Cat has alredy been rescued!"<<endl;
                                return false;
                            }

                            //remove the cat carrier from the inventory
                            if(!removeItem(inventory, inventoryCount, itemSymbol))
                           {
                                cout << "Failed to remove item from inventory!" << endl;
                                return false;
                            } 

                            //set objectives[OBJ_CAT_SECURED]=true;
                            objectives[OBJ_CAT_SECURED]=true;

                            //update the delta score vlue
                            deltaScore=SCORE_SECURE_CAT;
                        }


                        //if item symbol is cabinet train key
                        if(itemSymbol==ENT_CABINET_TRAIN_KEY)
                        {
                            //check if player is at required location
                            if(currentFloor!=5 || currentRoom!=2)
                            {
                                cout<<"Player must be at Room 2 in 5th Floor!"<<endl;
                                return false;
                            }

                            //check if objectives[OBJ_TRAIN_PREPARED] is already true
                            if(objectives[OBJ_TRAIN_PREPARED])
                            {
                                cout<<"Train has alredy been prepared!"<<endl;
                                return false;
                            }

                            //set objectives[OBJ_TRAIN_PREPARED]=true;
                            objectives[OBJ_TRAIN_PREPARED]=true;


                            //update the delta score vlue
                            deltaScore=SCORE_PREPARE_TRAIN;
                        }
                         


                            //update the score and resources
                            applyScoreDelta(score, deltaScore);
                            applyResourceDelta(energy, time, deltaEnergy, deltaTime);

                            return true;

                      }





bool handleUnlockCabinetCommand(const char* code, int currentFloor, int currentRoom,
                                bool objectives[NUM_OBJECTIVES], int& score)
                                {
                                    //check if the player is at required location
                                    if(currentFloor!=5 || currentRoom!=1)
                                    {
                                        cout<<"Player must be at Room 1, in 5th floor!"<<endl;
                                        return false;
                                    }
                                     
                                    if(!verifyCabinetCode(code, objectives))
                                    {
                                        cout<<"Invalid code or missing clues!"<<endl;
                                        return false;
                                    }

                                     //set the objective true
                                     objectives[OBJ_CABINET_UNLOCKED]=true;

                                     //update score
                                     applyScoreDelta(score, SCORE_CABINET_UNLOCK);

                                     
                                    cout<<"Cabinet unlocked successfully!"<<endl;

                                     return true;

                                }





bool checkEndConditions(int energy, int time, int &score, bool objectives[NUM_OBJECTIVES])
{
    int deltascore=0;
    
    //check wining cnditions
    if(objectives[OBJ_PASSENGER_RESCUED] && objectives[OBJ_CAT_SECURED] && objectives[OBJ_TRAIN_PREPARED])
    {
        cout<<"Game Won"<<endl;

      
        if(!objectives[OBJ_MISSION_WON])
        {
         objectives[OBJ_MISSION_WON]=true;

         //update score
         deltascore=SCORE_MISSION_VICTORY;
         applyScoreDelta(score, deltascore);
        }

        return true;
    }

//check loosing conditions
    if(energy==0 || time==0)
    {
        cout<<"Game lost!"<<endl;

        objectives[OBJ_MISSION_FAILED]=true;
        return true;
    }

    return false;
}