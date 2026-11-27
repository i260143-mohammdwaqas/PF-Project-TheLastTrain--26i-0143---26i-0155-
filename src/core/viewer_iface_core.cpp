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

void renderRoomDetails(int floorNum, int room,
                       const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                       int entityCount)
                       {
                        
                        if(floorNum<1 || floorNum>NUM_FLOORS){
                                cout << "Invalid floor!" <<endl;
                                return;}
                           
                        if(room<1 || room>ROOMS_PER_FLOOR){
                                cout<<"Invalid room!"  <<endl;
                                return;}
                        if(entityCount < 0 || entityCount > MAX_ENTITIES_PER_ROOM){
                                cout<<"Invalid entity count!"  <<endl;
                                return;}
                        cout<<"\nInspecting Floor "<<floorNum<<", Room "<<room<<endl;

                        if(entityCount==0){
                            cout<<"Nothing here!"<<endl;
                        }else{
                            for(int i=0; i<entityCount; i++)
                              {
                                 cout<<"- "<<getEntityName(roomEntities[floorNum-1][room-1][i])<<endl;
                              }}
                       }

void renderAsciiBuilding(int playerFloor, int playerRoom)
{
                        if(playerFloor<1 || playerFloor>NUM_FLOORS){
                                cout << "Invalid floor!" <<endl;
                                return;}
                           
                        if(playerRoom<1 || playerRoom>ROOMS_PER_FLOOR){
                                cout<<"Invalid room!"  <<endl;
                                return;}
                        cout<<"\n--- Building Map ---"<<endl;
                        for (int f = NUM_FLOORS; f >= 1; f--)
                        {
                            cout<<"Floor "<<f<<" ";
                            for(int r=1; r<= ROOMS_PER_FLOOR; r++){
                                if(f==playerFloor && r==playerRoom){
                                    cout<<"[ YOU ] ";
                                }else{
                                    cout<<"[F"<<f<<"-R"<<r<<"] ";
                                }

                            }
                            cout<<endl;
                        }

}

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