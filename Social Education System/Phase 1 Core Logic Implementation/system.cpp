#include "system.hpp"
#include "consts.hpp"

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
    istringstream iss(line);
    string command;
    iss >> command;

    char checkMark = ' ';
    iss >> checkMark;

    string restOfLine;
    getline(iss, restOfLine);

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
    for(User* user : students){
        if(stoi(data[ID]) == user->getID()){
            if(data[PASSWORD] == user->getPass()){
                cout << OK << endl;
                SignedInUser = user;
                return;
            } else throw(PERMISSION_DENIED);
        }
    }
    for(User* user : professors){
        if(stoi(data[ID]) == user->getID()){
            if(data[PASSWORD] == user->getPass()){
                cout << OK << endl;
                SignedInUser = user;
                return;
            } else throw(PERMISSION_DENIED);
        }
    }
    if(stoi(data[ID]) == admin.getID()){
        if(admin.getPass() == data[PASSWORD]){
            cout << OK << endl;
            SignedInUser = &admin;
            return;
        } else throw(PERMISSION_DENIED);
    }

    throw(NOT_FOUND);
}


void System::logOut(){
    SignedInUser = nullptr;
    cout << OK << endl;
    return;
}


void System::Delete(string line){
    istringstream iss(line);
    string command;
    iss >> command;

    char checkMark = ' ';
    iss >> checkMark;

    string restOfLine;
    getline(iss, restOfLine);

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
    istringstream iss(line);
    string command;
    iss >> command;

    char checkMark = ' ';
    iss >> checkMark;

    string restOfLine;
    getline(iss, restOfLine);

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
        if(Student* student = dynamic_cast<Student*>(SignedInUser)){
            student->displayCourse();
        } 
        else throw(PERMISSION_DENIED);
    }
    else 
        throw(NOT_FOUND);
}

void System::getPersonalPageInfo(string line){
    istringstream iss(line);
    string index , endOfLine;
    string userID = " ";
    iss >> index >> userID >> endOfLine;

    if(index == ID && endOfLine=="") {
        if(isInvalidArithmeticNumber(userID))
            throw(BAD_REQUEST);

        for(User* user : students){
            if(user->getID() == stoi(userID)){
                user->printInfo(majors);
                return;
            }
        }
        for(User* user : professors){
            if(user->getID() == stoi(userID)){
                user->printInfo(majors);
                return;
            }
        }
        if(admin.getID() == stoi(userID)){
            admin.printInfo(majors);
            return;
        }
        throw(NOT_FOUND);
    } else 
        throw(BAD_REQUEST);
}

void System::getPostInfo(string line){
    istringstream iss(line);

    map<string, string> data = getData(line, 2, ID, POST_ID);

    if(data.size() != 2 || isInvalidArithmeticNumber(data[ID]) || isInvalidNaturalNumber(data[POST_ID]))
        throw(BAD_REQUEST);

    for(User* user : students){
        if(user->getID() == stoi(data[ID])){
            user->printPostInfo(stoi(data[POST_ID]), majors);
            return;
        }
    }
    for(User* user : professors){
        if(user->getID() == stoi(data[ID])){
            user->printPostInfo(stoi(data[POST_ID]), majors);
            return;
        }
    }
    if(admin.getID() == stoi(data[ID])){
        admin.printPostInfo(stoi(data[POST_ID]), majors);
        return;
    }
    throw(NOT_FOUND);
}

void System::connect(string line){
    istringstream iss(line);
    string index , endOfLine;
    string followingID = " ";
    iss >> index >> followingID >> endOfLine;

    if(index == ID && endOfLine==""){
        if(isInvalidNaturalNumber(followingID))
            throw(BAD_REQUEST);

        for(auto& user : students){
            if((user->getID() == stoi(followingID)) && (user->getID() != SignedInUser->getID())){
                SignedInUser->follow(user);
                return;
            }
        }
        for(auto& user : professors){
            if((user->getID() == stoi(followingID)) && (user->getID() != SignedInUser->getID())){
                SignedInUser->follow(user);
                return;
            }
        }
        throw(NOT_FOUND);
    }
    else 
        throw(BAD_REQUEST);
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
    addCourseSub(data, professor->getName());
}

void System::addCourseSub(map<string, string> data, string professorName){
    coursesSubmission.push_back({courseSubIndex++, courseName(stoi(data[COURSE_ID]), coursesInfo), 
    stoi(data[CAPACITY]), professorName, data[TIME], data[EXAM_DATE], stoi(data[CLASS_NUMBER])});
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

void System::Put(string line){
    istringstream iss(line);
    string command;
    iss >> command;

    char checkMark = ' ';
    iss >> checkMark;

    string restOfLine;
    getline(iss, restOfLine);
    
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