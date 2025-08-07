#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define STARTING_LINE 0
#define STARTING_STATION 1
#define ENDING_LINE 2
#define ENDING_STATION 3

vector<int> inputUser();
void inputInfo(vector<vector<int>>& lines);
void solve(vector<vector<int>>& lines, vector<int> user);
int time_calculator(vector<vector<int>>& lines,const int endingLine, int beginningStation, int endingStation);
int station_in_destination(vector<vector<int>> lines,int endingLine, int beginningLine);
int checkReiteration(vector<int> checkedLines, int line);
int getMinimum_Time(vector<int> compareTimes);
int find_minimum_time(vector<vector<int>>& lines,const int& endingLine, int beginningLine, int beginningStation, int endingStation, vector<int> checkedLines, int time=0);

int main(){
    int numOfLines;
    cin >> numOfLines;

    vector<vector<int>> lines(numOfLines);
    inputInfo(lines);
    vector<int> user = inputUser();

    solve(lines, user);
}

void solve(vector<vector<int>>& lines, vector<int> user){
    vector<int> checkedLines;
    int minimumTime = find_minimum_time(lines,user[ENDING_LINE]-1,user[STARTING_LINE]-1,user[STARTING_STATION]-1,user[ENDING_STATION]-1,checkedLines);
    cout << minimumTime << endl;
}

void inputInfo(vector<vector<int>>& lines){
    for (int i=0; i<lines.size(); i++)
    {
        int stations;
        cin >> stations;
        for(int j=0; j<stations; j++){
            int station;
            cin >> station;
            lines[i].push_back(station);
        }
    } 
}

vector<int> inputUser(){
    vector<int> user(4);
    cin >> user[STARTING_LINE] >> user[STARTING_STATION] >> user[ENDING_LINE] >> user[ENDING_STATION];
    return user;
}

int time_calculator(vector<vector<int>>& lines,const int endingLine, int beginningStation, int endingStation)
{
    int time=0;
    for(int i=0; i<lines.size();i++){
        if(i==endingLine)
        {
            if(endingStation > beginningStation)
            {
                for(int j=beginningStation+1; j<lines[endingLine].size(); j++){
                    time++;
                    if (j==endingStation)
                    {
                        return time;
                    }
                }
            }
            else if(endingStation < beginningStation)
            {
                for(int j=beginningStation-1; j>=0; j--){
                    time++;
                    if (j==endingStation)
                    {
                        return time;
                    }
                }
            }
            else return 0;
        }
    }
    return 0;
}

int station_in_destination(vector<vector<int>> lines,int endingLine, int beginningLine){
    for(int i=0; i<lines[endingLine].size(); i++){
        if(lines[endingLine][i]-1==beginningLine)
            return i;
    }
    return 0;
}

int checkReiteration(vector<int> checkedLines, int line){
    for(int i=0; i<checkedLines.size(); i++){
        if(checkedLines[i]==line)
            return 1;
    }
    return 0;
}

int getMinimum_Time(vector<int> compareTimes){
    if(compareTimes.size()==0)
        return 0;
    int min=compareTimes[0];
    for(int i=0; i<compareTimes.size(); i++)
    {
        if(compareTimes[i] < min)
            min=compareTimes[i];
    }
    return min;
}

int find_minimum_time(vector<vector<int>>& lines,const int& endingLine, int beginningLine, int beginningStation, int endingStation, vector<int> checkedLines, int time){
    int temp_1=time;
    int temp_2=time;
    vector<int> compareTimes;
    checkedLines.push_back(beginningLine);

    if(beginningLine==endingLine){
        int t = time_calculator(lines, endingLine, beginningStation, endingStation);
        return t + time;
    }

    for(int i=beginningStation+1; i < lines[beginningLine].size(); i++)
    {
        temp_1++;
        if((lines[beginningLine][i]>0) && (checkReiteration(checkedLines,lines[beginningLine][i]-1) != 1))
        {
            int check = find_minimum_time(lines, endingLine, lines[beginningLine][i]-1, station_in_destination(lines,lines[beginningLine][i]-1,beginningLine), endingStation, checkedLines, temp_1);
            if(check!=0){
                int temp = 2 + check;
                compareTimes.push_back(temp);
            }  
        }
    }

    for(int i=beginningStation-1; i>=0; i--)
    {
        temp_2++;
        if((lines[beginningLine][i]>0) && (checkReiteration(checkedLines,lines[beginningLine][i]-1) != 1))
        {
            int check = find_minimum_time(lines, endingLine, lines[beginningLine][i]-1, station_in_destination(lines,lines[beginningLine][i]-1,beginningLine), endingStation, checkedLines, temp_2);
            if(check!=0){
                int temp = 2 + check;
                compareTimes.push_back(temp);
            }
        }
    }

    return getMinimum_Time(compareTimes);
}