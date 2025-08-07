#include "student.hpp"
#include "professor.hpp"
#include "consts.hpp"

Student::Student(int sem, int userID_, string name_, int majorID_, string password_) :
User(userID_, name_, majorID_, password_) {
    semester = sem;
}

void Student::printInfo(const vector<Major>& majors){
    string major = majorName(majors, majorID);

    cout << name << " " << major << " " << semester << " ";
    printCourses();
    cout << endl;
    for(int i = posts.size()-1; i >= 0; i--)
        cout << posts[i].index << " " << posts[i].title << endl;
}

void Student::printPostInfo(int postID, const vector<Major>& majors){
    string major = majorName(majors, majorID);

    Post post = findPost(posts, postID);

    cout << name << " " << major << " " << semester << " ";
    printCourses();
    cout << endl << post.index << " " << post.title << " " << post.message << endl;
}

void Student::checkNewCourse(string line, vector<CourseSubmission>& coursesSubmission, const vector<CourseInfo>& coursesInfo){
    istringstream iss(line);
    string index , endOfLine;
    string courseID = " ";
    iss >> index >> courseID >> endOfLine;

    if(index == ID && endOfLine == NOTHING){
        if(isInvalidNaturalNumber(courseID))
            throw(BAD_REQUEST);

        CourseSubmission newCourse = findCourse(coursesSubmission, stoi(courseID));
        int cID  = courseId(newCourse.courseName, coursesInfo);
        checkValidation(newCourse, cID, coursesInfo);
        addCourse(newCourse);
        joinCourse(coursesSubmission, stoi(courseID));
        sendNotification({userID, name, NOTIFICATIONS[2]});
    }
    else 
        throw(BAD_REQUEST);
}

void Student::checkValidation(CourseSubmission newCourse, int cID, const vector<CourseInfo>& coursesInfo){
    checkPrerequisite(semester, newCourse.courseName, coursesInfo);
    checkMajor(majorID, cID, coursesInfo);
    checkCoursesTime(this, newCourse.time);
    checkCourseExam(this, newCourse.examDate);
}

void Student::joinCourse(vector<CourseSubmission>& coursesSubmission, int courseID){
    for(auto &cS : coursesSubmission){
        if(cS.id == courseID)
            cS.members.push_back(this); 
    }
}

void Student::deleteCourse(string line){
    string index , endOfLine, courseID = " ";
    parseAloneInput(line, index, courseID, endOfLine);

    validateInput(index, courseID, endOfLine);
    deleteTheCourse(courseID);
    sendNotification({userID, name, NOTIFICATIONS[3]});
}

void Student::displayCourse(){
    if(courses.size() == 0)
        throw(EMPTY);
    
    for(auto c : courses)
        printCourseDetails(c);
}

void Student::requestTAform(string line, const vector<User*>& professors, vector<CourseSubmission>& coursesSubmission, const vector<CourseInfo>& coursesInfo){
    map<string, string> data = getData(line, 2, PROFESSOR_ID, FORM_ID);

    if(isInvalidNaturalNumber(data[PROFESSOR_ID]) || isInvalidNaturalNumber(data[FORM_ID]))
        throw(BAD_REQUEST);

    if(Professor* professor = dynamic_cast<Professor*>(findUser(professors, stoi(data[PROFESSOR_ID]))))
        professor->newTArequest(this, stoi(data[FORM_ID]), coursesSubmission, semester, coursesInfo);
    else
        throw(NOT_FOUND);
}