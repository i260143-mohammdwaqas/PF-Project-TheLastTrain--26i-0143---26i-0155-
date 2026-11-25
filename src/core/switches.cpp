#include "switches.h"
#include "util.h"
#include "physics.h"

// Door lock mechanics, lifts, and cabinet verification without structures

bool canUseLift1(int fromFloor, int toFloor)
{
    if((fromFloor == 1 && toFloor == 2)||(fromFloor == 2 && toFloor == 1))
        return true;

    return false;
}

bool canUseLift2(int fromFloor, int toFloor, bool lift2Operational, int emergencyTripsLeft)
{
   if(((fromFloor==2)||(fromFloor==4)||(fromFloor==5))&&((toFloor==2)||(toFloor==4)||(toFloor==5))&&(toFloor!=fromFloor)&&(lift2Operational==true||emergencyTripsLeft>0))
      return true;

   return false;   
}

bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount)
{
    return hasItem(inventory,inventoryCount,ENT_MASTER_KEY);
}

bool verifyCabinetCode(const char* enteredCode, const bool objectives[NUM_OBJECTIVES])
{
    if ((enteredCode == nullptr)||(objectives[OBJ_CLUE_2] == false)||(objectives[OBJ_CLUE_4] == false)||(objectives[OBJ_CLUE_7] == false))
    return false;
    if(enteredCode[0]=='4' && enteredCode[1]=='2' && enteredCode[2]=='7'&& enteredCode[3]=='\0' ){
        return true;
    }
    return false; 
    
}

bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                             bool objectives[NUM_OBJECTIVES], int& score)
{
    if (!hasItem(inventory, inventoryCount, ENT_FUSE) || objectives[OBJ_FUSE_INSTALLED]){
        return false;
    }else{
        removeItem(inventory, inventoryCount, ENT_FUSE);
        objectives[OBJ_FUSE_INSTALLED] = true;
        applyScoreDelta(score, SCORE_INSTALL_FUSE);
        return true;
    }
}


