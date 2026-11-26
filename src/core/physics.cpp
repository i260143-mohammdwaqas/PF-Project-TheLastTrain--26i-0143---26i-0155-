#include<iostream>
#include "physics.h"
#include "util.h"
#include "switches.h"
using namespace std;

//added myOwn functions isValidInventory and isValidEntity, to check wether an an item in the inventory or entity is valid??

// Movement validation and resource adjustments without structures

bool isVerticallyConnected(int fromF, int fromR, int toF, int toR,
                           char& outConnType)
                           {

                            
                            //check wether both rooms are direcly connnected vertically throuugh stairs
                            if((fromF==1 || fromF==2) && (toF==1 || toF==2) && fromR==1 && toR==1)
                            {
                                outConnType='S';
                                return true;
                            }

                            if((fromF==2 || fromF==3) && (toF==2 || toF==3) && fromR==1 && toR==1)
                            {
                                outConnType='S';
                                return true;
                            }

                            if((fromF==4 || fromF==5) && (toF==4 || toF==5) && fromR==1 && toR==1)
                            {
                                outConnType='S';
                                return true;
                            }

                            //lifts

                            //lift 1 connects floors 1, 2 and room 3, room 2
                            if(canUseLift1(fromF, toF) && fromR == toR &&
                                   (fromR == 2 || fromR == 3))
                                   {
                                     outConnType = '1';
                                     return true;
                                    }

                            // Lift 2
                            //we set true for operational and 0 for emergency trips left, because we are only checking the connection here, not the traversibility
                            if(canUseLift2(fromF, toF, true, 0) && fromR == 4 && toR == 4)
                           {
                            outConnType = '2';
                            return true;
                           }


                            return false;
                  }




bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
                         const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                         char& outConnType)
                         {
                            //initialize outConnType to empty
                             outConnType =' ';

                             //check wether the current floor and room are out of bounds, if yes return false
                             if (fromF < 1 || fromF > NUM_FLOORS || fromR < 1 || fromR > ROOMS_PER_FLOOR)
                             return false;
                             
                             //check whether the target floor and room are out of bounds, if yes return false
                             if (toF < 1 || toF > NUM_FLOORS || toR < 1 || toR > ROOMS_PER_FLOOR)
                             return false;


                            //check wether both the rooms are on the same floor, if yes, check if they are adjacent
                             //and set the outConnType accordingly
                             if(fromF==toF)
                             {  
                                //if from room is greater the to room
                                if(fromR==toR-1)
                                outConnType=horizontalConn[fromF-1][fromR-1];
                               
                                //if to room is greater than from room
                                else if(toR==fromR-1)
                                outConnType=horizontalConn[fromF-1][toR-1];

                                
                                //if rooms are not adjacent, while being on the same floor return false 
                                else 
                                return false;

                                 if(outConnType==' ' || outConnType=='#')
                                 return false;

                            }

                            //check if the rooms are vertically connected, if yes set the outConnType accordingly                   
                           else 
                            {
                                return isVerticallyConnected(fromF, fromR, toF, toR, outConnType);
                            }


                            return true;

                         }
                             




bool canTraverse(int targetFloor, int targetRoom, char connType,
                 const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                 int emergencyTripsLeft, bool lift2Operational)
                 {
                    //target floor and target room?
                    //door and windows are always traversible
                    if (connType== 'D' || connType== 'W' || connType== 'S' || connType == '1')
                        return true;
                     
                    if(connType=='L')
                    {
                        //see if the player has master key, if yes return true
                        if(unlockFloor3Door(inventory, inventoryCount))
                        return true;

                        //if no master key, return false, because bw adjacent rooms there is no other way                    
                        cout<<"Master Key is required to traverse the locked door!"<<endl;
                        return false;
                    }

                    //if connType is l2 check if lift is operational, if yes then return true;
                    //if lift 2 is not operational and emergency trips are left return true, else return false;
                    if(connType=='2')
                    {
                        if(lift2Operational)
                        return true;
                        else if(emergencyTripsLeft>0 && hasItem(inventory, inventoryCount, ENT_EMERGENCY_CARD))
                        {
                            return true;
                        }
                        else    
                        return false;
                    }

                    //all other connection types can not be transversed, or they are invalid
                    return false;
                 }
 

void applyResourceDelta(int& energy, int& time, int deltaEnergy, int deltaTime)
{
    //add change in energy to update enrgy value;
    energy+= deltaEnergy;

    //add change in time to update time value;
    time+= deltaTime;

  //clamp energy and time values to ensure, they remain within range;
  energy=clampInt(energy, 0, MAX_ENERGY);
  time=clampInt(time, 0, MAX_TIME);


}



void applyScoreDelta(int& score, int deltaScore)
{
    //add change in score to update score value;
    score+= deltaScore;

    //clamp score value to ensure, it remains within range;
    if(score<0)
    score=0;

}


bool isValidInventory(char itemSymbol)
{
    //check if item symbol is valid
    if(itemSymbol==ENT_CABINET_TRAIN_KEY|| itemSymbol==ENT_CAT_CARRIER || itemSymbol==ENT_EMERGENCY_CARD)
    return true;

    if(itemSymbol==ENT_FUSE || itemSymbol==ENT_LIFT_PASS || itemSymbol==ENT_MASTER_KEY || itemSymbol==ENT_MEDICAL_KIT)
    return true;

  

    return false;
}

bool isValidEntity(char itemSymbol)
{
    //check if item symbol is valid
    if(itemSymbol==ENT_CABINET_TRAIN_KEY || itemSymbol==ENT_CAT || itemSymbol==ENT_CAT_CARRIER || itemSymbol==ENT_CLUE_2)
    return true;

    if(itemSymbol==ENT_CLUE_7 || itemSymbol==ENT_CONTROL_PANEL || itemSymbol==ENT_EMERGENCY_CARD || itemSymbol==ENT_EMERGENCY_TOOLKIT)
    return true;

    if(itemSymbol==ENT_FUSE || itemSymbol==ENT_LIFT_PASS || itemSymbol==ENT_MASTER_KEY || itemSymbol==ENT_MEDICAL_KIT)
    return true;

    if(itemSymbol==ENT_PASSENGER || itemSymbol==ENT_OBSERVATION_TOKEN || itemSymbol==ENT_ENERGY_PACK || itemSymbol==ENT_CLUE_4)
    return true;

    return false;
}

bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, char itemSymbol)
{
    //vlidate
    if(inventoryCount < 0 || inventoryCount > MAX_INVENTORY_CAPACITY)
    {
    cout<<"Invalid inventory count!"<<endl;
    return false;
    }
    //check if the item is presnt in the the inventory, if yes return true;
    for(int i=0; i<inventoryCount; i++)
    if(inventory[i]==itemSymbol)
    return true;

    //if not, then return false;
    return false;

}




bool addItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol)
{
    //if the inventories are alredy full or out of bound return false
    if(inventoryCount>=MAX_INVENTORY_CAPACITY || inventoryCount<0)
    {cout<<"Inventory is Full, Collection failed!"<<endl;
    return false;}

    //if item alredy exists, return 0;
    if(hasItem(inventory, inventoryCount, itemSymbol))
    {cout<<"Item is alredy in the inventory, collection failed!"<<endl;
    return false;}

    //add the inventory symbol
    inventory[inventoryCount]=itemSymbol;
    inventoryCount++;

    return true;
}


bool removeItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol)
{
    int index=-1;

    if(inventoryCount < 0 || inventoryCount > MAX_INVENTORY_CAPACITY)
    {
    cout<<"Invalid inventory count!"<<endl;
    return false;
    }

    //search for that symbol
    for(int i=0; i<inventoryCount; i++)
    {
        if(inventory[i]==itemSymbol)
        {index=i;
            break;}

    }

    //if the inventory does not contain the item, return false;
    if(index==-1)
    {
        cout<<"Item is not present in the inventory!"<<endl;
        return false;}

    //shift the array to remove that item 
    for(int i=index; i<inventoryCount-1; i++)
    {
        inventory[i]=inventory[i+1];
    }

    inventoryCount--;

    return true;

}




                         