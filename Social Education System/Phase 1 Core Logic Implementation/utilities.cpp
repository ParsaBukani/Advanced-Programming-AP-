#include "utilities.hpp"
#include "user.hpp"  
#include "readFile.hpp" 
#include <cstring>

void checkAccess(User* SignedInUser, char mark){
    if(mark != '?')
        throw("Bad Request");
    if(SignedInUser == nullptr)
        throw("Permission Denied");
}

void checkEndOfLine(string endOfLine){
    for(char c : endOfLine){
        if(!isspace(c))
            throw("Bad Request");
    }
}


map<string, string> getData(string line, int numberOfReq, string arg1, string arg2, string arg3, string arg4, string arg5, string arg6){
    istringstream iss(line);
    map<string, string> data;
    string index;
    while (iss >> index) {
        if ((index == arg1 || index == arg2 || index == arg3 || index == arg4 || index == arg5 || index == arg6) && (data.size() < numberOfReq)){
            string value = " ";
            iss >> value;
            data[index] = value;
        }
        else
            throw("Bad Request");
    }
    return data;
}

map<string, string> getMessageAndTitle(string line){
    string index;
    map<string, string> data;

    bool isReadingValue = false;
    int quotation = 0;
    bool isFirstSpaceInLine = true;
    
    string substring = "";
    for(auto x : line){
        if(isReadingValue){
            if(x == '"'){
                substring = substring + x;
                quotation++;
            }
            else if(quotation == 1){
                substring = substring + x;
            }
            if(quotation == 2){
                isReadingValue = false;
                quotation = 0;
                data[index] = substring;
                substring = "";
            }
        }
        else if(x == ' '){
            if(isFirstSpaceInLine){
                isFirstSpaceInLine = false;
                continue;
            }
            if(substring == "title" || substring == "message") {
                index = substring;
                substring = "";
                isReadingValue = true;
                isFirstSpaceInLine = true;
            } else 
                throw("Bad Request");
        }
        else
            substring = substring + x;
    }

    if(data.size() != 2)
        throw("Bad Request");
    return data;
}

bool isInvalidArithmeticNumber(string number){
    for(char c : number){
        if(c < 48 || c > 57)
            return true;
    }
    return false;
}
bool isInvalidNaturalNumber(string str) {
    if(str.size()==0)
        return true;
    if(str[0] == 48)
        return true;
    for(char c : str){
        if(c < 48 || c > 57)
            return true;
    }
    return false;
}

string majorName(vector<Major> majors, int majorID){
    for(Major m : majors){
        if(m.mid == majorID)
            return m.name;
    }
    throw("Not Found");
}

string courseName(int courseID, vector<CourseInfo> coursesInfo){
    for(CourseInfo course : coursesInfo){
        if(course.cid == courseID)
            return course.name;
    }
    throw("Not Found");
}

int courseId(string courseName, const vector<CourseInfo>& coursesInfo){
    for(CourseInfo course : coursesInfo){
        if(course.name == courseName)
            return course.cid;
    }
    throw("Not Found");
}

void checkMajor(int majorID, int courseID, vector<CourseInfo> coursesInfo){
    for(auto course : coursesInfo){
        if(course.cid == courseID){
            for(auto major : course.majorsID){
                if(majorID == major)
                    return;
            }
            break;
        }
    }
    throw("Permission Denied");
}

void checkCoursesTime(User* user, string time){
    for(auto c : user->getCourses()){
        if(haveTimeConflict(c.time, time))
            throw("Permission Denied");
    }
}

bool haveTimeConflict(const std::string& time1, const std::string& time2) {
    string day1, day2;
    int startHour1, endHour1, startHour2, endHour2;
    string s1, s2, e1, e2;

    istringstream iss1(time1), iss2(time2);

    getline(iss1, day1, ':');
    getline(iss1, s1, '-');
    iss1 >> e1;

    getline(iss2, day2, ':');
    getline(iss2, s2, '-');
    iss2 >> e2;

    startHour1 = stoi(s1);
    endHour1 = stoi(e1);
    startHour2 = stoi(s2);
    endHour2 = stoi(e2);

    if (day1 != day2) 
        return false;

    if (((endHour1 <= endHour2) && (endHour1 > startHour2)) || ((startHour1 < endHour2) && (startHour1 >= startHour2)) || ((startHour1 >= startHour2) && (endHour1 <= endHour2)) || ((startHour1 <= startHour2) && (endHour1 >= endHour2)))
        return true;

    return false;
}

void checkCourseExam(User* user, string examDate){
    for(auto c : user->getCourses()){
        if(haveExamTimeConflict(c.examDate, examDate))
            throw("Permission Denied");
    }
}

bool haveExamTimeConflict(const string& time1, const string& time2){
    if(time1 == time2)
        return true;
    return false;
}

void printCourseDetails(const CourseSubmission& cS) {
    cout << cS.id << " " << cS.courseName << " " << cS.capacity << " " << cS.professor 
         << " " << cS.time << " " << cS.examDate << " " << cS.classNumber << endl;
}

CourseSubmission findCourse(const vector<CourseSubmission>& coursesSubmission, int id){
    for(auto cS : coursesSubmission){
        if(cS.id == id){
            return cS;
        }
    }
    throw("Not Found");
}

void findAndPrintCourse(const vector<CourseSubmission>& coursesSubmission, int id){
    CourseSubmission cS = findCourse(coursesSubmission, id);
    printCourseDetails(cS);
}

void checkPrerequisite(int semester, string courseName, const vector<CourseInfo>& coursesInfo){
    for(auto course : coursesInfo){
        if(course.name == courseName){
            if(semester < course.prerequisite)
                throw("Permission Denied");
            break;
        }
    }
}

CourseSubmission findCourse(int ID, vector<CourseSubmission> courses){
    for(auto cS : courses){
        if(cS.id == ID)
            return cS;
    }
    throw("Not Found");
}