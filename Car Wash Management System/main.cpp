#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

struct PassTimeResult
{
    int time;
    int carID;
    string perviousPosition;
    int perviousPositionID;
    string currentPosition;
    int currentPositionID;

    bool operator==(const PassTimeResult& other) const {
        return (time == other.time) && (carID == other.carID);
    }
};

const int DONE = 0;

class Worker;
class Stage;
class Car;
class CarWashManager;

bool compareWorkers(Worker* a, Worker* b);
bool compareCars(Car* a, Car* b);
bool compareNewCars(pair<Car*, int> a, pair<Car*, int> b);
bool compareResults(PassTimeResult result1, PassTimeResult result2);

int findNextStage(vector<int> carStages, int currentStage);

string tokenize(char index, int placing, string line);
inline void omitTrashCharForLinux(string& str, const char ch = '\r');

void readStagesFile(const string name, vector<Stage>& stages);
void readWorkersFile(const string name, vector<Worker>& workers);
void process(vector<Stage> stages, vector<Worker> workers);
void printResult(vector<PassTimeResult> result);


class Car {
    private:
        int id;
        vector<int> stages;
        string status;
        bool isNew = false;
        int previousStageID;
    public:
        Car(int i_, vector<int> s_){
            id = i_;
            stages = s_;
        }
        int getID(){ return id; }
        void setStatus(string s){ status = s; }
        vector<int> getStages(){ return stages; }
        bool getStatue(){ return isNew; }
        void setStatue(bool statue){ isNew = statue; }
        void setpreviousStageID(int p_){ previousStageID = p_; }
        int getPreviousStageID(){ return previousStageID; }
        string getStatues(){ return status; }
};

class Worker {
    private:
        int id;
        int stageID;
        int timeToFinish;
        Car* currentCar;
    public:
        Worker(int i_, int s_, int t_){
            id = i_;
            stageID = s_;
            timeToFinish = t_;
            currentCar = nullptr;
        };
        void renewWorker(){ currentCar = nullptr; }
        void setCar(Car* c){ currentCar = c; }
        int getStageID(){ return stageID; }
        Car* getWorkingCar(){ return currentCar; }
        int getTimeToFinish(){ return timeToFinish; }
        int getID(){ return id; }
        bool isWorkerReady(){
            if(currentCar == nullptr)
                return true;
            else return false;
        }
        int getWorkingCarID(){ return currentCar->getID(); }
};

class Stage {
    private:
        int id;
        int price;
        int numOfWashedCars = 0;
        int income = 0;
        vector<Car*> queue;
        vector<Car*> carInStage;
        vector<Worker*> workers;
        vector<pair<Car*,int>> newCarsAdded;
    public:
        Stage(int i_, int p_){
            id = i_;
            price = p_;
        };

        void addWorker(Worker* w){ workers.push_back(w); }
        void addCarToStage(Car* c){ carInStage.push_back(c); }
        void addCarToQueue(Car* c){ queue.push_back(c); }
        int getID(){ return id; }
        void clearCarsInStage(){ carInStage = vector<Car*>(); }
        vector<Car*> getCarInStage(){ return carInStage; }

        vector<PassTimeResult> renewQueue(int time);
        vector<PassTimeResult> manageNewCars(int time);

        bool isWorkerFree(){
            for(int i=0; i<workers.size(); i++){
                if (workers[i]->getWorkingCar() == nullptr)
                    return true;
            }
            return false;
        }
        void addNewCar(Car* newCar, int previousStageID){ newCarsAdded.push_back({newCar, previousStageID}); }
        vector<Worker*> getWorkers(){ return workers; }
        void setNumOfWashedCars(int n){ numOfWashedCars += n; }
        int getNumOfWashedCars(){ return numOfWashedCars; }
        int getNumOfCarsInQueue(){ return queue.size(); }
        int getNumOfCarsBeingWashed(){ return carInStage.size(); }
        int getIncome(){ return price*numOfWashedCars; }
};

class CarWashManager {
    public:
        CarWashManager(vector<Stage> s, vector<Worker> w){
            stages = s;
            workers = w;
        }
        void carArrival(Car* newCar);
        void passTime(int t);
        void getStageStatus(int stageID);
        void getWorkerStatus(int workerID);
        void getCarStatus(int carID);

        vector<PassTimeResult> renewStages(int time);
        vector<Worker> getWorkers(){ return workers; }
        vector<Stage> getStages(){ return stages; }
        void putWorkersInStages();                                   
    private:
        vector<Stage> stages;
        vector<Worker> workers;
        vector<Car*> cars;
        int time = 0;  
};

void CarWashManager::getCarStatus(int carID){
    bool found = false;
    for(auto car : cars){
        if (car->getID() == carID)
        {
            if(car->getStatues() == "Done")
                cout << "Done" << endl;
            else
                cout << car->getStatues() << ": " << car->getPreviousStageID() << endl;
            found = true;
        }
    }
    if(!found)
        cout << "NOT FOUND" << endl;
}


void CarWashManager::getWorkerStatus(int workerID)
{
    bool found = false;
    for(auto worker : workers){
        if(worker.getID() == workerID){
            if(worker.isWorkerReady())
                cout << "Idle" << endl;
            else
                cout << "Working: " << worker.getWorkingCarID() << endl;
            found = true;
        }
    }
    if(!found)
        cout << "NOT FOUND" << endl;
}

void CarWashManager::getStageStatus(int stageID){
    bool found = false;
    for(auto stage : stages){
        if(stage.getID() == stageID){
            cout << "Number of washed cars: " << stage.getNumOfWashedCars() << endl;
            cout << "Number of cars in queue: " << stage.getNumOfCarsInQueue() << endl;
            cout << "Number of cars being washed: " << stage.getNumOfCarsBeingWashed() << endl;
            cout << "Income: " << stage.getIncome() << endl;
            found = true;
        }
    }
    if(!found)
        cout << "NOT FOUND" << endl;
}

void CarWashManager::carArrival(Car* newCar){
    cars.push_back(newCar);
    for(int i=0; i<stages.size(); i++){
        if (stages[i].getID() == newCar->getStages()[0]){
            if (stages[i].isWorkerFree())
            {
                vector<Worker*> workers = stages[i].getWorkers();
                Worker* w = *min_element(workers.begin(), workers.end(), compareWorkers);
                w -> setCar(newCar);
                newCar -> setStatus("In service");
                newCar->setpreviousStageID(stages[i].getID());          
                stages[i].addCarToStage(newCar);
                cout << time << " Car " << newCar->getID() << ": Arrived -> Stage " << stages[i].getID() << endl;
            }else
            {
                newCar -> setStatus("In line");
                newCar->setpreviousStageID(stages[i].getID());           
                stages[i].addCarToQueue(newCar);
                cout << time << " Car " << newCar->getID() << ": Arrived -> Queue " << stages[i].getID() << endl;
            }
        }        
    }
}

void CarWashManager::passTime(int t)
{
    for(int i=0; i<t; i++){
        time += 1;
        vector<PassTimeResult> result;
        result = renewStages(time);
        printResult(result);
    }
}

vector<PassTimeResult> CarWashManager::renewStages(int time)
{
    vector<PassTimeResult> result;
    
    for(int j=0; j<stages.size(); j++)
    {
        vector<Car*> carInStage = stages[j].getCarInStage();
        stages[j].setNumOfWashedCars(carInStage.size());

        for(int i=0; i<carInStage.size(); i++)
        {
            vector<int> carStages = carInStage[i]->getStages();
            int nextStageID = findNextStage(carStages, stages[j].getID());

            if(nextStageID == DONE)
            {
                result.push_back({time, carInStage[i]->getID(), "Stage", stages[j].getID(), "Done", DONE});
                
                carInStage[i]->setStatus("Done");
            }
            else
            {
                carInStage[i]->setStatue(true);
                carInStage[i]->setpreviousStageID(stages[j].getID());
                for(int k=0; k<stages.size(); k++){
                    if(stages[k].getID() == nextStageID){
                        stages[k].addCarToQueue(carInStage[i]);
                        break; 
                    }
                }      
            }
        }
        stages[j].clearCarsInStage();
    }
    
    for(int j=0; j<stages.size(); j++){
        vector<PassTimeResult> result2 = stages[j].renewQueue(time);
        result.insert(result.end(), result2.begin(), result2.end());
    }

    return result;
}

void printResult(vector<PassTimeResult> result)
{
    if(result.size()==0)
        return;

    PassTimeResult r = *min_element(result.begin(), result.end(), compareResults);

    if(r.currentPositionID == DONE)
        cout << r.time << " Car " << r.carID << ": " << r.perviousPosition << " " << r.perviousPositionID << " -> " << r.currentPosition << endl;
    else
        cout << r.time << " Car " << r.carID << ": " << r.perviousPosition << " " << r.perviousPositionID << " -> " << r.currentPosition << " " << r.currentPositionID << endl;
    
    auto pos = find(result.begin(), result.end(), r);
        result.erase(pos);

    printResult(result);
}

vector<PassTimeResult> Stage::renewQueue(int time)
{
    for(int i=0; i<workers.size(); i++){
        workers[i] -> renewWorker();
    }
    
    vector<PassTimeResult> result;
    while (isWorkerFree() && (queue.size() > 0))
    {
        Worker* w = *min_element(workers.begin(), workers.end(), compareWorkers);
        Car* c = *min_element(queue.begin(), queue.end(), compareCars);
        if(c->getStatue() == true)
            result.push_back({time, c->getID(), "Stage", c->getPreviousStageID(), "Stage", id});
        else
            result.push_back({time, c->getID(), "Queue", id, "Stage", id});

        c -> setStatus("In service");
        c -> setpreviousStageID(id);
        addCarToStage(c);

        auto pos = find(queue.begin(), queue.end(), c);
        queue.erase(pos);
        
        w -> setCar(c);
    }
    
    for(int i=0; i<queue.size(); i++){
        queue[i]->setpreviousStageID(id);
        queue[i]->setStatus("In line");
        if(queue[i]->getStatue() == true)
            queue[i]->setStatue(false);
    }
    return result;
}

int findNextStage(vector<int> carStages, int currentStage)
{
    for(int i=0; i<carStages.size(); i++){
        if(carStages[i] == currentStage && (i+1 < carStages.size()))
            return carStages[i+1];
    }
    return DONE;
}

int main(int argc,char* argv[])
{
    const string STAGES = argv[1];
    const string WORKERS = argv[2];
    vector<Stage> stages;
    vector<Worker> workers;
    readStagesFile(STAGES, stages);
    readWorkersFile(WORKERS, workers);
    process(stages, workers);
}

string tokenize(char index, int placing, string line)
{
    line += index;
    int counter = 1;
    string substring = "";
    for(auto x : line){
        if(x==index)
        {
            if(counter==placing)
                return substring;
            else{
                counter++;
                substring = "";
            }
        }
        else
            substring = substring + x;
    }
}

inline void omitTrashCharForLinux(string& str, const char ch){
    int index = str.find(ch);
    if (index != string::npos)
        str.erase(index, 1);
}

void readStagesFile(const string name, vector<Stage>& stages)
{
    ifstream file(name);
    string line;
    int i = 0;

    getline(file, line);
    while(getline(file,line)){
        omitTrashCharForLinux(line);

        int id = stoi(tokenize(',', 1, line));
        int price = stoi(tokenize(',', 2, line));
        stages.push_back(Stage(id, price));
        i++;
    }
    file.close();
}

void readWorkersFile(const string name, vector<Worker>& workers)
{
    ifstream file(name);
    string line;
    int i = 0;

    getline(file, line);
    while(getline(file,line)){
        omitTrashCharForLinux(line);

        int id = stoi(tokenize(',', 1, line));
        int stageID = stoi(tokenize(',', 2, line));
        int timeToFinish = stoi(tokenize(',', 3, line));
        workers.push_back(Worker(id, stageID, timeToFinish));
        i++;
    }
    file.close();
}

void CarWashManager::putWorkersInStages(){                            
    for(int i=0; i<workers.size(); i++){
        for(int j=0; j<stages.size(); j++){
            if (workers[i].getStageID() == stages[j].getID())
            {
                stages[j].addWorker(&workers[i]);
            }
        }
    }
}

void process(vector<Stage> stages, vector<Worker> workers)
{
    CarWashManager carWashManager(stages, workers);
    carWashManager.putWorkersInStages();                             
    int carID = 1;

    string line;
    while (getline(cin, line))
    {
        istringstream iss(line);
        string command;
        iss >> command;

        if (command == "pass_time")
        {
            int time;
            if (iss >> time)
                carWashManager.passTime(time);
            else
                cerr << "Invalid input for time. Please enter an integer." << endl;
        }
        else if (command == "car_arrival")
        {
            vector<int> stages;
            int stage;
            while (iss >> stage){
                stages.push_back(stage);
            }
            carWashManager.carArrival(new Car(carID, stages));
            carID++;
        }
        else if (command == "get_stage_status")
        {
            int stageID;
            if(iss >> stageID)
                carWashManager.getStageStatus(stageID);
            else
                cerr << "Invalid input for stageID. Please enter an integer." << endl;
        }
        else if (command == "get_worker_status")
        {
            int workerID;
            if(iss >> workerID)
                carWashManager.getWorkerStatus(workerID);
            else
                cerr << "Invalid input for workerID. Please enter an integer." << endl;
        }
        else if (command == "get_car_status")
        {
            int carID;
            if(iss >> carID)
                carWashManager.getCarStatus(carID);
            else
                cerr << "Invalid input for carID. Please enter an integer." << endl;
        }
        else
            cerr << "Unknown command: " << command << ". Skipping." << endl;
    }
}

bool compareWorkers(Worker* a, Worker* b)
{
    if(a->getWorkingCar()!=nullptr || b->getWorkingCar()!= nullptr)
        return false;
    if (a->getTimeToFinish( )== b->getTimeToFinish())
        return a->getID() < b->getID();
    else
        return a->getTimeToFinish() < b->getTimeToFinish();
}

bool compareCars(Car* a, Car* b)
{
    return a->getID() < b->getID();
}

bool compareNewCars(pair<Car*, int> a, pair<Car*, int> b)
{
    return a.first->getID() < b.first->getID();
}

bool compareResults(PassTimeResult result1, PassTimeResult result2){
    if(result1.time != result2.time)
        return result1.time < result2.time;
    return result1.carID < result2.carID;
}