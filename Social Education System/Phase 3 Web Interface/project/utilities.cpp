#include "utilities.hpp"
#include "user.hpp"  
#include "readFile.hpp" 
#include "consts.hpp"
#include <cstring>

const char QUOTATION = '"';

void checkAccess(User* SignedInUser, char mark){
    if(mark != EXCLAMATION_MARK)
        throw(BAD_REQUEST);
    if(SignedInUser == nullptr)
        throw(PERMISSION_DENIED);
}

void checkEndOfLine(string endOfLine){
    for(char c : endOfLine){
        if(!isspace(c))
            throw(BAD_REQUEST);
    }
}

User* findUser(const vector<User*>& users, int id){
    for(auto user : users){
        if(user->getID() == id)
            return user;
    }
    return nullptr;
}

bool checkPassword(User* user, string password){
    if(password == user->getPass()){
        cout << OK << endl;
        return true;
    } else throw(PERMISSION_DENIED);
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
            throw(BAD_REQUEST);
    }
    return data;
}

map<string, string> getMessageAndTitle(string line){
    map<string, string> data;
    istringstream iss(line);
    string index, value;

    while (iss >> index) {
        if(index == IMAGE || index == MESSAGE || index == TITLE || index == ID){
            iss >> ws;
            if(iss.peek() == QUOTATION){
                iss.ignore();
                getline(iss, value, QUOTATION);
                value = "\"" + value + "\"";
            } else
                iss >> value;

            data[index] = value;
        } 
        else
            throw(BAD_REQUEST);
    }

    if (data[TITLE].empty() || data[MESSAGE].empty())
        throw(BAD_REQUEST);
    
    return data;
}

map<string, string> getMessageAndCourseID(string line){
    map<string, string> data;
    istringstream iss(line);
    string index, value;

    while (iss >> index) {
        if(index == COURSE_ID || index == MESSAGE){
            iss >> ws;
            if(iss.peek() == QUOTATION){
                iss.ignore();
                getline(iss, value, QUOTATION);
                value = "\"" + value + "\"";
            } else
                iss >> value;

            data[index] = value;
        } 
        else
            throw(BAD_REQUEST);
    }

    if (data[COURSE_ID].empty() || data[MESSAGE].empty())
        throw(BAD_REQUEST);
    if(isInvalidNaturalNumber(data[COURSE_ID]))
        throw(BAD_REQUEST);

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
    throw(NOT_FOUND);
}

string courseName(int courseID, vector<CourseInfo> coursesInfo){
    for(CourseInfo course : coursesInfo){
        if(course.cid == courseID)
            return course.name;
    }
    throw(NOT_FOUND);
}

int courseId(string courseName, const vector<CourseInfo>& coursesInfo){
    for(CourseInfo course : coursesInfo){
        if(course.name == courseName)
            return course.cid;
    }
    throw(NOT_FOUND);
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
    throw(PERMISSION_DENIED);
}

void checkCoursesTime(User* user, string time){
    for(auto c : user->getCourses()){
        if(haveTimeConflict(c.time, time))
            throw(PERMISSION_DENIED);
    }
}

bool haveTimeConflict(const std::string& time1, const std::string& time2) {
    string day1, day2;
    int startHour1, endHour1, startHour2, endHour2;
    string s1, s2, e1, e2;

    istringstream iss1(time1), iss2(time2);

    getline(iss1, day1, ':'); getline(iss1, s1, '-'); iss1 >> e1;

    getline(iss2, day2, ':'); getline(iss2, s2, '-'); iss2 >> e2;

    startHour1 = stoi(s1); endHour1 = stoi(e1);
    startHour2 = stoi(s2); endHour2 = stoi(e2);

    if (day1 != day2) 
        return false;

    if (((endHour1 <= endHour2) && (endHour1 > startHour2)) || ((startHour1 < endHour2) && (startHour1 >= startHour2)) || ((startHour1 >= startHour2) && (endHour1 <= endHour2)) || ((startHour1 <= startHour2) && (endHour1 >= endHour2)))
        return true;

    return false;
}

void checkCourseExam(User* user, string examDate){
    for(auto c : user->getCourses()){
        if(haveExamTimeConflict(c.examDate, examDate))
            throw(PERMISSION_DENIED);
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
        if(cS.id == id)
            return cS;
    }
    throw(NOT_FOUND);
}

Post findPost(const vector<Post>& posts, int postID){
    for(auto p : posts){
        if(p.index == postID)
            return p;
    }
    throw(NOT_FOUND);
}

void findAndPrintCourse(const vector<CourseSubmission>& coursesSubmission, int id){
    CourseSubmission cS = findCourse(coursesSubmission, id);
    printCourseDetails(cS);
}

void checkPrerequisite(int semester, string courseName, const vector<CourseInfo>& coursesInfo){
    for(auto course : coursesInfo){
        if(course.name == courseName){
            if(semester <= course.prerequisite)
                throw(PERMISSION_DENIED);
            break;
        }
    }
}

void sendNotificationToMembers(CourseSubmission& cS, int senderID){
    for (auto member : cS.admins){
        if(member.first->getID() != senderID)
            member.first->getNotification({cS.id, cS.courseName, NOTIFICATIONS[5]});
    }
    for (auto member : cS.members)
        member->getNotification({cS.id, cS.courseName, NOTIFICATIONS[5]});
}

void checkPostPermission(const CourseSubmission& cS, User* user){
    for(auto admin : cS.admins){
        if(admin.first == user && admin.second == true)
            return;
    }
    throw(PERMISSION_DENIED);
}

int findCourseIDinPosts(const vector<Post>& posts, int formId, string& courseName){
    for(auto p : posts){
        if(p.index == formId && p.courseID != 0){
            courseName = p.course;
            return p.courseID;
        }
    }
    throw(NOT_FOUND);
}

void checkCourseInvolved(const string courseName, const vector<CourseSubmission>& courses){
    for(auto c : courses){
        if(c.courseName == courseName)
            return;
    }
    throw(PERMISSION_DENIED);
}

void parseLine(string line, string& command, string& restOfLine, char& checkMark){
    istringstream iss(line);
    iss >> command >> checkMark;
    getline(iss, restOfLine);
}

void parseAloneInput(string line, string& index, string& id, string& endOfLine){
    stringstream iss(line);
    iss >> index >> id >> endOfLine;
}

void validateInput(const string& index, const string& id, const string& endOfLine){
    if (index == ID && endOfLine == NOTHING) {
        if (isInvalidNaturalNumber(id))
            throw(BAD_REQUEST);

        return;
    } 
    else throw(BAD_REQUEST);
}

bool printUserInfo(const vector<User*>& users, int userID, const vector<Major>& majors){
    for(User* user : users){
        if(user->getID() == userID){
            user->printInfo(majors);
            return true;
        }
    }
    return false;
}

bool printUserPostInfo(const vector<User*>& users, int userID, int postID, const vector<Major>& majors){
    for(User* user : users){
        if(user->getID() == userID){
            user->printPostInfo(postID, majors);
            return true;
        }
    }
    return false;
}