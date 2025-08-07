#include "system.hpp"
#include "consts.hpp"

const char* OK = "OK";
const char* EMPTY = "Empty";
const char* NOT_FOUND = "Not Found";
const char* BAD_REQUEST = "Bad Request";
const char* PERMISSION_DENIED = "Permission Denied";

const char EXCLAMATION_MARK = '?';

const string ID = "id";
const string PASSWORD = "password";
const string POST_ID = "post_id";
const string COURSE_ID = "course_id";
const string PROFESSOR_ID = "professor_id";
const string CAPACITY = "capacity";
const string TIME = "time";
const string EXAM_DATE = "exam_date";
const string CLASS_NUMBER = "class_number";
const string FORM_ID = "form_id";

const string NOTHING = "";


System::System(vector<User*> s_, vector<User*> p_, vector<CourseInfo> c_, vector<Major> m_) {
    students = s_;
    professors = p_;
    coursesInfo = c_;
    majors = m_;
    for(auto user : students)
        admin.acceptFollow(user);
    for(auto user : professors)
        admin.acceptFollow(user);
}

void System::run(){
    while(!std::cin.eof())
    {
        string line;
        if(!std::getline(cin, line))
            break;

        istringstream iss(line);
        string command;
        iss >> command;

        string restOfLine;
        getline(iss, restOfLine);
        
        try {
            if(command == POST)
                Post(restOfLine);
            else if(command == GET)
                Get(restOfLine);
            else if(command == PUT)
                Put(restOfLine);
            else if(command == DELETE)
                Delete(restOfLine);
            else
                throw(BAD_REQUEST);
        }
        catch (const char* message){
            cout << message << endl; 
        }
    }
}

void System::Post(string line){
    string command, restOfLine;
    char checkMark = ' ';
    parseLine(line, command, restOfLine, checkMark);

    if(command == LOGIN){
        if(checkMark != EXCLAMATION_MARK)
            throw(BAD_REQUEST);
        if(SignedInUser != nullptr)
            throw(PERMISSION_DENIED);
        logIn(restOfLine);
    }
    else if(command == LOGOUT){
        checkEndOfLine(restOfLine);
        checkAccess(SignedInUser, checkMark);
        logOut();
    }
    else if(command == SUB_POST){
        checkAccess(SignedInUser, checkMark);
        SignedInUser->createPost(restOfLine);
        cout << OK << endl;
    }
    else if(command == CONNECT){
        checkAccess(SignedInUser, checkMark);
        connect(restOfLine);
        cout << OK << endl;
    }
    else if(command == COURSE_OFFER){
        if(checkMark != EXCLAMATION_MARK)
            throw(BAD_REQUEST);
        if(SignedInUser != &admin)
            throw(PERMISSION_DENIED);
        courseOffer(restOfLine);
        cout << OK << endl;
    }
    else if(command == PROFILE_PHOTO){
        checkAccess(SignedInUser, checkMark);
        SignedInUser->setProfilePhoto(restOfLine);
        cout << OK << endl;
    }
    else if(command == COURSE_POST){
        checkAccess(SignedInUser, checkMark);
        coursePost(restOfLine);
        cout << OK << endl;
    }
    else if(command == TA_FORM){
        checkAccess(SignedInUser, checkMark);
        if(Professor* teacher = dynamic_cast<Professor*>(SignedInUser)){
            teacher->createTAform(restOfLine, coursesSubmission);
            cout << OK << endl;
        } else throw(PERMISSION_DENIED);
    }
    else if(command == TA_REQUEST){
        checkAccess(SignedInUser, checkMark);
        if(Student* student = dynamic_cast<Student*>(SignedInUser)){
            student->requestTAform(restOfLine, professors, coursesSubmission, coursesInfo);
            cout << OK << endl;
        } else throw(PERMISSION_DENIED);
    }
    else if(command == CLOSE_TA_FORM){
        checkAccess(SignedInUser, checkMark);
        if(Professor* teacher = dynamic_cast<Professor*>(SignedInUser))
            teacher->closeTAform(restOfLine, coursesSubmission);
        else 
            throw(PERMISSION_DENIED);
    }
    else 
        throw(NOT_FOUND);
}

void System::logIn(string line){
    istringstream iss(line);

    map<string, string> data = getData(line, 2, ID, PASSWORD);

    if(data.size() != 2 || isInvalidArithmeticNumber(data[ID]))
        throw(BAD_REQUEST);
    
    checkLogIn(data);
}

void System::checkLogIn(map<string, string> data) {
    User* student = findUser(students, stoi(data[ID]));   
    if(student != nullptr){
        if(checkPassword(student, data[PASSWORD])){
            SignedInUser = student;
            return;
        }
    }
    User* professor = findUser(professors, stoi(data[ID])); 
    if(professor != nullptr){
        if(checkPassword(professor, data[PASSWORD])){
            SignedInUser = professor;
            return;
        }
    }
    if(stoi(data[ID]) == admin.getID()){
        if(checkPassword(&admin, data[PASSWORD])){
            SignedInUser = &admin;
            return;
        }
    }
    throw(NOT_FOUND);
}

void System::logOut(){
    SignedInUser = nullptr;
    cout << OK << endl;
    return;
}

void System::Delete(string line){
    string command, restOfLine;
    char checkMark = ' ';
    parseLine(line, command, restOfLine, checkMark);

    if(command == SUB_POST){
        checkAccess(SignedInUser, checkMark);
        SignedInUser->deletePost(restOfLine);
        cout << OK << endl;
    }
    else if(command == MY_COURSES){
        if(checkMark != EXCLAMATION_MARK)
            throw(BAD_REQUEST);
        if(Student* student = dynamic_cast<Student*>(SignedInUser)){
            student->deleteCourse(restOfLine);
            cout << OK << endl;
        } 
        else throw(PERMISSION_DENIED);
    }
    else 
        throw(NOT_FOUND);
}

void System::Get(string line){
    string command, restOfLine;
    char checkMark = ' ';
    parseLine(line, command, restOfLine, checkMark);

    if(command == PERSONAL_PAGE){
        checkAccess(SignedInUser, checkMark);
        getPersonalPageInfo(restOfLine);
    }
    else if(command == SUB_POST){
        checkAccess(SignedInUser, checkMark);
        getPostInfo(restOfLine);
    }
    else if(command == NOTIFICATION){
        checkEndOfLine(restOfLine);
        checkAccess(SignedInUser, checkMark);
        SignedInUser->showNotifications();
    }
    else if(command == COURSES){
        checkAccess(SignedInUser, checkMark);
        displayCourses(restOfLine);
    }
    else if(command == MY_COURSES){
        if(checkMark != EXCLAMATION_MARK)
            throw(BAD_REQUEST);
        checkEndOfLine(restOfLine);
        if(Student* student = dynamic_cast<Student*>(SignedInUser))
            student->displayCourse();
        else throw(PERMISSION_DENIED);
    }
    else if(command == COURSE_CHANNEL){
        checkAccess(SignedInUser, checkMark);
        displayCourseChannel(restOfLine);
    }
    else if(command == COURSE_POST){
        checkAccess(SignedInUser, checkMark);
        displayPostChannel(restOfLine);
    }
    else 
        throw(NOT_FOUND);
}

void System::getPersonalPageInfo(string line){
    string index , endOfLine;
    string userID = " ";
    parseAloneInput(line, index, userID, endOfLine);

    if(index == ID && endOfLine == NOTHING) {
        if(isInvalidArithmeticNumber(userID))
            throw(BAD_REQUEST);

        if(printUserInfo(students, stoi(userID), majors))
            return;
        else if(printUserInfo(professors, stoi(userID), majors))
            return;
        else if(admin.getID() == stoi(userID))
            admin.printInfo(majors);
        else 
            throw(NOT_FOUND);
    } 
    else throw(BAD_REQUEST);
}

void System::getPostInfo(string line){
    map<string, string> data = getData(line, 2, ID, POST_ID);

    if(data.size() != 2 || isInvalidArithmeticNumber(data[ID]) || isInvalidNaturalNumber(data[POST_ID]))
        throw(BAD_REQUEST);

    if(printUserPostInfo(students, stoi(data[ID]), stoi(data[POST_ID]), majors))
        return;
    else if(printUserPostInfo(professors, stoi(data[ID]), stoi(data[POST_ID]), majors))
        return;
    else if(admin.getID() == stoi(data[ID]))
        admin.printPostInfo(stoi(data[POST_ID]), majors);
    else 
        throw(NOT_FOUND);
}

void System::connect(string line){
    string index , endOfLine, followingID = " ";
    parseAloneInput(line, index, followingID, endOfLine);
    validateInput(index, followingID, endOfLine);

    if(follow(students, stoi(followingID)))
        return;
    else if(follow(professors, stoi(followingID)))
        return;
    else 
        throw(NOT_FOUND);
}

bool System::follow(vector<User*> users, int followingID){
    for(auto& user : users){
        if((user->getID() == followingID) && (user->getID() != SignedInUser->getID())){
            SignedInUser->follow(user);
            return true;
        }
    }
    return false;
}

void System::courseOffer(string line){
    istringstream iss(line);

    map<string, string> data = getData(line, 6, COURSE_ID, PROFESSOR_ID, CAPACITY, TIME, EXAM_DATE, CLASS_NUMBER);
    
    if(data.size() != 6 || isInvalidNaturalNumber(data[COURSE_ID]) || isInvalidNaturalNumber(data[PROFESSOR_ID]) || isInvalidNaturalNumber(data[CAPACITY]) || isInvalidNaturalNumber(data[CLASS_NUMBER]))
        throw(BAD_REQUEST);

    for(User* user : students){
        if(user->getID() == stoi(data[PROFESSOR_ID]))
            throw(PERMISSION_DENIED);
    }
    Professor* professor = nullptr;
    for(User* p : professors){
        if(p->getID() == stoi(data[PROFESSOR_ID]))
            professor = dynamic_cast<Professor*>(p);
    }

    if(professor == nullptr)
        throw(NOT_FOUND);
    
    admin.offerCourse(data, professor, coursesInfo, courseSubIndex);
    addCourseSub(data, professor);
}

void System::addCourseSub(map<string, string> data, User* teacher){
    CourseSubmission cS = {courseSubIndex++, courseName(stoi(data[COURSE_ID]), coursesInfo), 
    stoi(data[CAPACITY]), teacher->getName(), data[TIME], data[EXAM_DATE], stoi(data[CLASS_NUMBER])};
    cS.admins.push_back({teacher, true});
    coursesSubmission.push_back(cS);
}

void System::displayCourses(string line){
    istringstream iss(line);
    string index , id, endOfline;
    iss >> index >> id >> endOfline;

    if(index == ID){
        checkEndOfLine(endOfline);
        if(isInvalidNaturalNumber(id))
            throw(BAD_REQUEST);
        findAndPrintCourse(coursesSubmission, stoi(id));
        return;
    }

    checkEndOfLine(index);

    if(coursesSubmission.size() == 0)
        cout << EMPTY << endl;
    
    for(auto cS : coursesSubmission)
        cout << cS.id << " " << cS.courseName << " " << cS.capacity << " " << cS.professor << endl;
}

void System::displayCourseChannel(string line){
    istringstream iss(line);
    string index , id, endOfLine;
    iss >> index >> id >> endOfLine;

    if(index == ID && endOfLine == NOTHING){
        if(isInvalidNaturalNumber(id))
            throw(BAD_REQUEST);
        CourseSubmission cS = findCourse(coursesSubmission, stoi(id));

        for(auto ad : cS.admins){
            if(ad.first == SignedInUser && ad.second == true){
                printCourseChannel(cS);
                return;
            }
        }
        for(auto ad : cS.members){
            if(ad == SignedInUser){
                printCourseChannel(cS);
                return;
            }
        }
        throw(PERMISSION_DENIED);
    }
    else 
        throw(BAD_REQUEST);
}

void System::displayPostChannel(string line){
    map<string, string> data = getData(line, 2, ID, POST_ID);

    if(isInvalidNaturalNumber(data[ID]) || isInvalidNaturalNumber(data[POST_ID]))
        throw(BAD_REQUEST);
    CourseSubmission cS = findCourse(coursesSubmission, stoi(data[ID]));

    for(auto ad : cS.admins){
        if(ad.first == SignedInUser && ad.second == true){
            printPostChannel(cS, stoi(data[POST_ID]));
            return;
        }
    }
    for(auto ad : cS.members){
        if(ad == SignedInUser){
            printPostChannel(cS, stoi(data[POST_ID]));
            return;
        }
    }
    throw(PERMISSION_DENIED);
}

void System::printCourseChannel(const CourseSubmission& cS){
    printCourseDetails(cS);
    for(int i = cS.posts.size()-1; i >= 0; i--)
        cout << cS.posts[i].index << " " << cS.posts[i].author << " " << cS.posts[i].title << endl;
}

void System::printPostChannel(const CourseSubmission& cS, int postID){
    for(auto p : cS.posts){
        if(p.index == postID){
            printCourseDetails(cS);
            cout << p.index << " " << p.author << " " << p.title << " " << p.message << endl;
            return;
        }
    }
    throw(NOT_FOUND);
}

void System::Put(string line){
    string command, restOfLine;
    char checkMark = ' ';
    parseLine(line, command, restOfLine, checkMark);
    
    if(command == MY_COURSES){
        if(checkMark != EXCLAMATION_MARK)
            throw(BAD_REQUEST);
        if(Student* student = dynamic_cast<Student*>(SignedInUser)){
            student->checkNewCourse(restOfLine, coursesSubmission, coursesInfo);
            cout << OK << endl;
        } 
        else throw(PERMISSION_DENIED);
    }
    else 
        throw(NOT_FOUND);
}

void System::coursePost(string line){
    map<string, string> data = getMessageAndTitle(line);
    
    if(isInvalidNaturalNumber(data[ID]))
        throw(BAD_REQUEST);
    
    SignedInUser->createPostOnChannel(data, coursesSubmission);
}