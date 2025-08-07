#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

const string NONE = "all";
const string NOTHING = "";
const string EQUALLY = "equally";
const string UNEQUALLY = "unequally";
const string ADJUSTMENT = "adjustment";
const char TRASH_CHAR = '\r';
const int LEFT_FIRST = 1;
const int RIGHT_FIRST = 2;
const int BOTH_EQUALL = 3;
const int TYPE = 1;
const int PAYERS = 2;
const int BORROWERS = 3;
const int NAME = 1;
const int EXPENCE = 2;

struct  member
{
    string name;
    double credit=0;
};
struct result
{
    string debtor;
    string creditor;
    double amount;
};

void readFile(const string name, vector<member>& members);
int numOfMembers(const string name);
void process(const string spendings, vector<member> members, vector<result>& result);
string tokenize(char index, int placing, string line);
double roundDown(double number);
void calculateExpenses(vector<member>& members, string spendings);
void manageEqually(vector<member>& members, string payers, string borrowers);
void manageUnequally(vector<member>& members, string payers, string borrowers);
void manageAdjustment(vector<member>& members, string payers, string borrowers);
double allMoney(string line);
void calculateMeans(double money, int numOfBorrows, double& mean, double& lastOneDebt);
void calculatePays(vector<member>& members, string line);
void calculateEquallDebts(vector<member>& members, string borrowers, double money, int numOfBorrows);
void calculateAdjustmentDebts(vector<member>& members, string borrowers, double money);
void calculateDebts(vector<member>& members, string line);
string lastMember(string line);
vector<pair<string,double>> putDebtorsInOrder(vector<member> members);
vector<pair<string,double>> putCreditorsInOrder(vector<member> members);
void seperateDebtors(vector<member> members, vector<pair<string, double>>& debtors);
void seperateCreditors(vector<member> members, vector<pair<string, double>>& creditors);
void orderDebtNumbers(vector<pair<string, double>>& debtors);
void orderCreditNumbers(vector<pair<string, double>>& creditors);
int checkNameOrder(string a, string b);
void orderNames(vector<pair<string, double>>& vector);
void optimizeReciept(vector<result>& result, vector<pair<string, double>>& debtors, vector<pair<string,double>>& creditors, int debtor=0, int creditor=0);
void orderResultAmounts(vector<result>& results);
void printResult(vector<result> results);
inline void omitTrashCharForLinux(string& str, const char ch = TRASH_CHAR);

int main(int argc,char* argv[])
{
    const string NAMES = argv[1];
    const string SPENDINGS = argv[2];
    vector<member> members;
    vector<result> result;

    readFile(NAMES ,members);
    process(SPENDINGS ,members ,result);
    printResult(result);

    return 0;
}

string tokenize(char index, int placing, string line)
{
    line += index;
    int counter = 1;
    string substring = NOTHING;
    for(auto x : line){
        if(x==index)
        {
            if(counter==placing)
                return substring;
            else{
                counter++;
                substring = NOTHING;
            }
        }
        else
            substring = substring + x;
    }
    return NONE;
}

double roundDown(double number){
    number = 100*number;
    int decimalNumber = number;
    return (double)decimalNumber/100;
}

int numOfMembers(const string name)
{
    ifstream names(name);
    int numOfMembers = 0;
    string line;

    getline(names, line);
    while(getline(names,line)){
        numOfMembers++;
    }
    names.close();
    return numOfMembers;
}

inline void omitTrashCharForLinux(string& str, const char ch){
    int index = str.find(ch);
    if (index != string::npos)
        str.erase(index, 1);
}

void readFile(const string name, vector<member>& members)
{
    ifstream names(name);
    members.resize(numOfMembers(name));
    string line;
    int i = 0;

    getline(names, line);
    while(getline(names,line))
    {
        omitTrashCharForLinux(line);
        members[i].name=line;
        i++;
    }
    names.close();
}

void process(const string spendings, vector<member> members, vector<result>& result)
{
    calculateExpenses(members, spendings);

    vector<pair<string,double>> debtors = putDebtorsInOrder(members);
    vector<pair<string,double>> creditors = putCreditorsInOrder(members);
    
    optimizeReciept(result, debtors, creditors);
}

void calculateExpenses(vector<member>& members, string spending)
{
    ifstream spendings(spending);
    string line;
    while(getline(spendings,line))
    {
        string type = tokenize(',', TYPE, line);
        string payers = tokenize (',', PAYERS, line);
        string borrowers = tokenize(',', BORROWERS, line);
        
        if(type == EQUALLY)
            manageEqually(members, payers, borrowers);
        if(type == UNEQUALLY)
            manageUnequally(members, payers, borrowers);
        if(type == ADJUSTMENT)
            manageAdjustment(members, payers, borrowers);
    }
    spendings.close();
}

double allMoney(string line)
{
    double allPayments = 0;
    int place = 1;
    string element = tokenize(';', place, line);

    while (element != NONE)
    {
        double expense = stod(tokenize(':', EXPENCE, element));
        allPayments += expense;
        place++;
        element = tokenize(';', place, line);
    }
    return allPayments;
}

int numOfBorrowers(string line)
{
    if (line == NOTHING)
        return 0;
    int count = 0;
    int place = 1;

    string element = tokenize(';', place, line);
    while (element != NONE)
    {
        count++;
        place++;
        element = tokenize(';', place, line);
    }
    return count;
}

void manageEqually(vector<member>& members, string payers, string borrowers)
{
    calculatePays(members, payers);
    calculateEquallDebts(members, borrowers, allMoney(payers), numOfBorrowers(borrowers));
}

void manageUnequally(vector<member>& members, string payers, string borrowers)
{
    calculatePays(members, payers);
    calculateDebts(members, borrowers);
}

void manageAdjustment(vector<member>& members, string payers, string borrowers)
{
    calculatePays(members, payers);
    calculateDebts(members, borrowers);
    calculateAdjustmentDebts(members, borrowers, allMoney(payers)-allMoney(borrowers));
}

void calculatePays(vector<member>& members, string line)
{
    int place = 1;
    string element = tokenize(';', place, line);
    while (element != NONE)
    {
        string name = tokenize(':', NAME, element);
        double expense = stod(tokenize(':', EXPENCE, element));

        for(int j=0; j<members.size(); j++)
        {
            if(members[j].name == name)
                members[j].credit += expense;
        }

        place++;
        element = tokenize(';', place, line);
    }
}

void calculateDebts(vector<member>& members, string line)
{
    int place = 1;
    string element = tokenize(';', place, line);
    while (element != NONE)
    {
        string name = tokenize(':', NAME, element);
        double expense = stod(tokenize(':', EXPENCE, element));

        for(int j=0; j<members.size(); j++)
        {
            if(members[j].name == name)
                members[j].credit -= expense;
        }
        place++;
        element = tokenize(';', place, line);
    }
}

void calculateMeans(double money, int numOfBorrows, double& mean, double& lastOneDebt){
    mean = (double)money/numOfBorrows;
    mean = roundDown(mean);
    lastOneDebt = money - (mean*(numOfBorrows-1));
}

void calculateEquallDebts(vector<member>& members, string borrowers, double money, int numOfBorrows)
{
    double mean;
    double lastOneDebt;

    if(numOfBorrows == 0){
        calculateMeans(money, members.size(), mean, lastOneDebt);

        for(int i=0; i<members.size(); i++){
            if(i == members.size()-1)
                members[i].credit -= lastOneDebt;
            else
                members[i].credit -= mean;
        }
    }
    else{
        calculateMeans(money, numOfBorrows, mean, lastOneDebt);

        int place = 1;
        string element = tokenize(';', place, borrowers);
        while (element != NONE)
        {
            for(int j=0; j<members.size(); j++){
                if(members[j].name == element){
                    if(element == lastMember(borrowers))
                        members[j].credit -= lastOneDebt;
                    else
                        members[j].credit -= mean;
                }
            }
            place++;
            element = tokenize(';', place, borrowers);
        }
    }
}

void calculateAdjustmentDebts(vector<member>& members, string borrowers, double money){
    double mean;
    double lastOneDebt;
    calculateMeans(money, members.size(), mean, lastOneDebt);

    for(int i=0; i<members.size(); i++){
        if(members[i].name == tokenize(':', NAME,(tokenize(';', numOfBorrowers(borrowers), borrowers))))
            members[i].credit -= lastOneDebt;
        else
            members[i].credit -= mean;
    }
}

string lastMember(string line){
    int place = 1;
    string final;

    string check = tokenize(';', place, line);
    while (check != NONE)
    {
        final = check;
        place++;
        check = tokenize(';', place, line);
    }
    return final;
}

vector<pair<string,double>> putDebtorsInOrder(vector<member> members)
{
    vector<pair<string,double>> debtors;
    
    seperateDebtors(members, debtors);
    orderDebtNumbers(debtors);
    orderNames(debtors);
    
    return debtors;
}

vector<pair<string,double>> putCreditorsInOrder(vector<member> members)
{
    vector<pair<string,double>> creditors;
    
    seperateCreditors(members, creditors);
    orderCreditNumbers(creditors);
    orderNames(creditors);
    
    return creditors;
}

void seperateDebtors(vector<member> members, vector<pair<string, double>>& debtors){
    for(int i=0; i<members.size(); i++){
        if(members[i].credit<0){
            debtors.push_back(make_pair(members[i].name,members[i].credit));
        }
    }
}

void seperateCreditors(vector<member> members, vector<pair<string, double>>& creditors){
    for(int i=0; i<members.size(); i++){
        if(members[i].credit>0){
            creditors.push_back(make_pair(members[i].name,members[i].credit));
        }
    }
}

void orderDebtNumbers(vector<pair<string, double>>& debtors){
    for(int i = 0; i < debtors.size(); i++){
        for(int j = i + 1; j < debtors.size(); j++){
            if(debtors[j].second < debtors[i].second){
                swap(debtors[i], debtors[j]);
            }
        }
    }
}

void orderCreditNumbers(vector<pair<string, double>>& creditors){
    for(int i = 0; i < creditors.size(); i++){
        for(int j = i + 1; j < creditors.size(); j++){
            if(creditors[i].second < creditors[j].second){
                swap(creditors[i], creditors[j]);
            }
        }
    }
}

int checkNameOrder(string a, string b){
    int i=0;
    while ( (i < a.length()) && (i < b.length()))
    {
        if(a[i] == b[i]){
            i++;
            continue;
        }
        if (a[i] < b[i])
            return LEFT_FIRST;
        else 
            return RIGHT_FIRST;   
    }
    return BOTH_EQUALL;  
}

void orderNames(vector<pair<string, double>>& vector){
    for(int i=0; i<vector.size(); i++){
        for (int j= i+1; j<vector.size(); j++)
        {
            if(vector[i].second == vector[j].second)
            {
                if(checkNameOrder(vector[i].first,vector[j].first) == LEFT_FIRST)
                    continue;
                else{
                    string temp = vector[i].first;
                    vector[i].first = vector[j].first;
                    vector[j].first = temp;
                }
            } 
        }  
    }
}

void optimizeReciept(vector<result>& result, vector<pair<string, double>>& debtors, vector<pair<string, double>>& creditors, int debtor, int creditor)
{
    if (debtor >= debtors.size() || creditor >= creditors.size()) 
        return;
    
    double balance = (debtors[debtor].second) + (creditors[creditor].second);

    if(balance > 0){
        result.push_back({debtors[debtor].first, creditors[creditor].first, -debtors[debtor].second});
        creditors[creditor].second = balance;
        optimizeReciept(result, debtors, creditors, debtor + 1, creditor);
    }
    if(balance < 0){
        result.push_back({debtors[debtor].first, creditors[creditor].first, creditors[creditor].second});
        debtors[debtor].second = balance;
        optimizeReciept(result, debtors, creditors, debtor, creditor + 1);
    }
    if(balance == 0){
        result.push_back({debtors[debtor].first, creditors[creditor].first, creditors[creditor].second});
        optimizeReciept(result, debtors, creditors, debtor + 1, creditor + 1);
    }
}

void printResult(vector<result> results)
{
    orderResultAmounts(results);

    for(int i=0; i<results.size(); i++){
        cout << results[i].debtor << " -> " << results[i].creditor << ": " << results[i].amount << endl;
    }
}

void orderResultAmounts(vector<result>& results)
{
    for(int i = 0; i < results.size(); i++){
        for(int j = i + 1; j < results.size(); j++){
            if(results[i].amount < results[j].amount){
                swap(results[i],results[j]);
            }
        }
    }
}