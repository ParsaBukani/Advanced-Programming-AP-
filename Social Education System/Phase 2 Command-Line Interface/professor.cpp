#include "professor.hpp"
#include "student.hpp"
#include "consts.hpp"

const string ACCEPT = "accept";
const string REJECT = "reject";
const string ACCEPT_COMMENT = "Your request to be a teaching assistant has been accepted";
const string REJECT_COMMENT = "Your request to be a teaching assistant has been rejected";

Professor::Professor(string pos, int userID_, string name_, int majorID_, string password_) :
User(userID_, name_, majorID_, password_) {
    position = pos;
}

void Professor::printInfo(const vector<Major>& majors){
    string major = majorName(majors, majorID);

    cout << name << " " << major << " " << position << " ";
    printCourses();
    cout << endl;

    for(int i = posts.size()-1; i >= 0 ; i--){
        if(posts[i].course != "")
            cout << posts[i].index << " TA form for " << posts[i].course << " course" << endl;
        else
            cout << posts[i].index << " " << posts[i].title << endl;
    }
}

void Professor::printPostInfo(int postID, const vector<Major>& majors){
    string major = majorName(majors, majorID);

    Post post = findPost(posts, postID);

    cout << name << " " << major << " " << position << " ";
    printCourses();

    if(post.course == ""){
        cout << endl << post.index << " " << post.title << " " << post.message << endl;
    } else {
        cout << endl << post.index << " TA form for " << post.course << " course" << endl;
        findAndPrintCourse(courses, post.courseID);
        cout << post.message << endl;
    }
}

void Professor::checkNewCourse(map<string, string> data, vector<CourseInfo>& coursesInfo, int courseSubIndex){
    string cName = courseName(stoi(data[COURSE_ID]), coursesInfo);
    
    checkMajor(majorID, stoi(data[COURSE_ID]), coursesInfo);
    checkCoursesTime(this, data[TIME]);
    
    CourseSubmission cS = {courseSubIndex, cName, stoi(data[CAPACITY]),
    name, data[TIME], data[EXAM_DATE], stoi(data[CLASS_NUMBER])};
    addCourse(cS);
}

void Professor::createTAform(string line, const vector<CourseSubmission>& coursesSubmission){
    map<string, string> data = getMessageAndCourseID(line);

    CourseSubmission course = findCourse(coursesSubmission, stoi(data[COURSE_ID]));
    checkCourseInvolved(course.courseName, courses);

    Post p = {postIndex++, NOTHING, data[MESSAGE], NOTHING, course.courseName, stoi(data[COURSE_ID])};
    posts.push_back(p);

    sendNotification({userID, name, NOTIFICATIONS[4]});
}

void Professor::newTArequest(User* user, int formID, vector<CourseSubmission>& coursesSubmission, int semester, const vector<CourseInfo>& coursesInfo){
    string courseName;
    int coursID = findCourseIDinPosts(posts, formID, courseName);
    
    checkPrerequisite(semester, courseName, coursesInfo);

    for(auto& cS : coursesSubmission){
        if(cS.id == coursID){
            cS.admins.push_back({user, false});
            return;
        }
    }
}

void Professor::closeTAform(string line, vector<CourseSubmission>& coursesSubmission){
    istringstream iss(line);
    string index , endOfLine;
    string formID = " ";
    iss >> index >> formID >> endOfLine;

    if(index == ID && endOfLine == NOTHING){
        if(isInvalidNaturalNumber(formID))
            throw(BAD_REQUEST);

        int courseID = findCourseIDinPosts(posts, stoi(formID), endOfLine);
        manageChannel(courseID, coursesSubmission);
        deleteTAform(stoi(formID));
    }
    else 
        throw(BAD_REQUEST);
}

void Professor::manageChannel(int courseID, vector<CourseSubmission>& coursesSubmission){
    for(auto& cS : coursesSubmission){
        if(cS.id == courseID){
            manageCourseChannel(cS);
            break;
        }
    }
}

void Professor::manageCourseChannel(CourseSubmission& cS){
    cout << "We have received " << numOfTArequests(cS) << " requests for the teaching assistant position" << endl;
    while(isTArequestLeft(cS)){
        Student* applicant = dynamic_cast<Student*>(getApplicant(cS));

        cout << applicant->getID() << " " << applicant->getName() << " " << applicant->getSemester() << ": ";
        string prompt = NOTHING;
        getline(cin, prompt);

        if(prompt == ACCEPT)
            acceptApplicant(cS, applicant);
        else if(prompt == REJECT)
            rejectApplicant(cS, applicant);
        else
            continue;
    }
}

int Professor::numOfTArequests(const CourseSubmission& cS){
    int counter = 0;
    for (auto req : cS.admins){
        if(req.second == false)
            counter++;
    }
    return counter;
}

bool Professor::isTArequestLeft(const CourseSubmission& cS){
    for (auto req : cS.admins){
        if(req.second == false)
            return true;
    }
    return false;
}

User* Professor::getApplicant(const CourseSubmission& cS){
    for (auto req : cS.admins){
        if(req.second == false)
            return req.first;
    }
    return nullptr;
}

void Professor::acceptApplicant(CourseSubmission& cS, User* user){
    for (auto& req : cS.admins){
        if(req.first == user){
            req.second = true;
            user->getNotification({cS.id, cS.courseName, ACCEPT_COMMENT});
        }
    }
}

void Professor::rejectApplicant(CourseSubmission& cS, User* user){
    for (auto& req : cS.admins){
        if(req.first == user){
            auto it = find(cS.admins.begin(), cS.admins.end(), req);
            cS.admins.erase(it);
            user->getNotification({cS.id, cS.courseName, REJECT_COMMENT});
        }
    }
}

void Professor::deleteTAform(int formID){
    for(auto p : posts){
        if(p.index == formID && p.course != NOTHING)
            deleteThePostOnly(p);
    }
}