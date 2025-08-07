#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct info
{
    string name;
    int folder;
    int line;
};
void treePrint(vector<info>& Items, int& depth, string spaces="", int Line=0, int reiteration=1);
bool check_nextExist(vector<info> Items, int Line);
vector<info> getItems(int numOfItems);

int main()
{   
    int numOfItems , depth;
    cin >> numOfItems >> depth;

    vector<info> Items = getItems(numOfItems);

    treePrint(Items,depth);
}   


vector<info> getItems(int numOfItems){
    vector<info> Items(numOfItems);
    for(int i=0; i<numOfItems; i++){
        cin >> Items[i].name >> Items[i].folder;
        Items[i].line=i+1;
    }
    return Items;
}


void treePrint(vector<info>& Items, int& depth, string spaces, int Line, int reiteration)//reiteration is used to calculate the depth
{
    if(reiteration>depth)
        return;
    int reLine=Line;

    if(check_nextExist(Items, Line)){
        spaces += "|  ";
    } else if (Line !=0){
        spaces += "   ";
    }

    for(int i=reLine; i<Items.size(); i++)
    {  
        if(Items[i].folder==Line){
            cout << spaces << "|__" << Items[i].name << endl;
            treePrint(Items, depth, spaces, Items[i].line, reiteration+1);
        }
        reLine++;
    }
    return;
}


bool check_nextExist(vector<info> Items, int Line)
{
    for(int i=Line; i<Items.size(); i++){
        if(Items[i].folder==Items[Line-1].folder)
            return true;
    }
    return false;
}