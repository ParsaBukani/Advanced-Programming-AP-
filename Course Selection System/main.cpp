#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct offeringCourses {
    string course_name;
    int unit;
    string prerequisites;
};
struct preTakenCourses {
    string course_name;
    int unit;
    double grade;
};

//Function prototyping:
int numOfCommas(string prerequisites);
string seperation(string prerequisites , int place);
double mean(vector<preTakenCourses> preTakenCourses);
int existance(vector<offeringCourses> availableCourse,vector<string> request);
int minimumValidation(vector<offeringCourses> availableCourse,vector<string> request);
int maximumValidation(vector<offeringCourses> availableCourse,vector<preTakenCourses> preTakenCourses,vector<string> request);
int checkTaken(vector<preTakenCourses> preTakenCourses,vector<string> request);
int checkPrerequisites(vector<offeringCourses> availableCourse,vector<preTakenCourses> preTakenCourses,vector<string> request);

int main()
{
    int m , n , k;//number of offers, preTakens_courses and requests

    cin >> m;
    vector<offeringCourses> availableCourse(m);
    for(int i=0 ; i<m ; i++)
    {
        cin >> availableCourse[i].course_name >> availableCourse[i].unit >> availableCourse[i].prerequisites;
    }

    cin >> n;
    vector<preTakenCourses> preTakenCourses(n);
    for(int i=0 ; i<n ; i++)
    {
        cin >> preTakenCourses[i].course_name >> preTakenCourses[i].unit >> preTakenCourses[i].grade;
    }

    cin >> k;
    vector<string> request(k);
    for(int i=0 ; i<k ;i++)
    {
        cin >> request[i];
    }
    
    if(existance(availableCourse,request)){
        cout << "Course Not Offered!" << endl;
        return 0;
    }

    if(minimumValidation(availableCourse,request)){
        cout << "Minimum Units Validation Failed!" << endl;
        return 0;
    }

    if(maximumValidation(availableCourse,preTakenCourses,request)){
        cout << "Maximum Units Validation Failed!" << endl;
        return 0;
    }

    if(checkTaken(preTakenCourses,request)){
        cout << "Course Already Taken!" << endl;
        return 0;
    }

    if(checkPrerequisites(availableCourse,preTakenCourses,request)){
        cout << "Prerequisites Not Met!" << endl;
        return 0;
    }

    cout << "OK!" << endl;
}


int numOfCommas(string prerequisites){
    int sum=0;
    for(char temp : prerequisites){
        if(temp==',')
        sum++;
    }
    return sum;
}


string seperation(string prerequisites , int place){
    int compare=0;
    string word="";

    for(char temp : prerequisites){
        if(temp==','){
            if(compare==place){
                return word;
            }
            compare++;
            word="";
        }
        else{
            word+=temp;
        }
    }
    return word;
}

int existance(vector<offeringCourses> availableCourse,vector<string> request){
    for(int i=0; i<request.size(); i++){
        int flag=0;
        for(int j=0; j<availableCourse.size(); j++){
            if(request[i]==availableCourse[j].course_name){
                flag=1;
                break;
            }
        }
        if(flag==0)
            return 1;
    }
    return 0;
}


int minimumValidation(vector<offeringCourses> availableCourse,vector<string> request){
    int sum=0;
    for(int i=0; i<request.size(); i++){
        for(int j=0; j<availableCourse.size(); j++){
            if(request[i]==availableCourse[j].course_name){
                sum+=availableCourse[j].unit;
            }
        }
    }
    if(sum>=12)
        return 0;
    else 
        return 1;    
}


double mean(vector<preTakenCourses> preTakenCourses){
    double sum=0;
    int coefficient=0;
    for(int i=0; i<preTakenCourses.size(); i++){
        sum+=preTakenCourses[i].unit*preTakenCourses[i].grade;
        coefficient+=preTakenCourses[i].unit;
    }
    return sum/coefficient;
}


int maximumValidation(vector<offeringCourses> availableCourse,vector<preTakenCourses> preTakenCourses,vector<string> request){
    if(mean(preTakenCourses)>=17){
        int sum=0;
        for(int i=0; i<request.size(); i++){
            for(int j=0; j<availableCourse.size(); j++){
                if(request[i]==availableCourse[j].course_name){
                    sum+=availableCourse[j].unit;
                }
            }
        }
        if(sum>24)
            return 1;
        else
            return 0;
    }
    else if((mean(preTakenCourses)>=12)&&(mean(preTakenCourses)<17)){
        int sum=0;
        for(int i=0; i<request.size(); i++){
            for(int j=0; j<availableCourse.size(); j++){
                if(request[i]==availableCourse[j].course_name){
                    sum+=availableCourse[j].unit;
                }
            }
        }
        if(sum>20)
            return 1;
        else
            return 0;
    }
    else{
        int sum=0;
        for(int i=0; i<request.size(); i++){
            for(int j=0; j<availableCourse.size(); j++){
                if(request[i]==availableCourse[j].course_name){
                    sum+=availableCourse[j].unit;
                }
            }
        }
        if(sum>14)
            return 1;
        else
            return 0;
    }
}


int checkTaken(vector<preTakenCourses> preTakenCourses,vector<string> request){
    for(int i=0; i<request.size(); i++){
        for(int j=0; j<preTakenCourses.size(); j++){
            if(request[i]==preTakenCourses[j].course_name){
                if(preTakenCourses[j].grade>=10)
                    return 1;
            }
        }
    }
    return 0;
}


int checkPrerequisites(vector<offeringCourses> availableCourse,vector<preTakenCourses> preTakenCourses,vector<string> request)
{
    for(int i=0; i<request.size(); i++){
        for(int j=0; j<availableCourse.size(); j++){
            if(request[i]==availableCourse[j].course_name){
                for(int k=0; k<numOfCommas(availableCourse[j].prerequisites)+1; k++){
                    int flag=0;
                    for(int o=0; o<preTakenCourses.size(); o++){
                        if(preTakenCourses[o].course_name==seperation(availableCourse[j].prerequisites,k) && (preTakenCourses[o].grade>=10)){
                            flag=1;
                            break;
                        }
                    }
                    if(flag==0)
                        return 1;
                }
            }
        }
    }
    return 0;
}