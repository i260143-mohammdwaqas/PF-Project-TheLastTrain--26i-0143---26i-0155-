#include<iostream>
#include "viewer_iface.h"
#include "util.h"
#include "level.h"
using namespace std;


// Terminal UI and dashboard rendering using primitive parameters
void renderDashboard(int playerFloor, int playerRoom, int energy, int time, int score,
                     const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                     int emergencyTripsLeft)
{
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
}

void renderRoomDetails(int floor, int room,
                       const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                       int entityCount)
                       {}

void renderAsciiBuilding(int playerFloor, int playerRoom)
{}

void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES])
{
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
}