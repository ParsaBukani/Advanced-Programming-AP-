#include "student.hpp"

Student::Student(int sem, int userID_, string name_, int majorID_, string password_) :
User(userID_, name_, majorID_, password_) {
    semester = sem;
}

void Student::printInfo(vector<Major> majors){
    string major = majorName(majors, majorID);

    cout << name << " " << major << " " << semester << " ";
    for(int i=0; i<courses.size(); i++){
        cout << courses[i].courseName;
        if(i != courses.size() - 1)
            cout << ',';
    }
    cout << endl;
    for(int i=0; i<posts.size(); i++)
        cout << posts[i].index << " " << posts[i].title << endl;
}

void Student::printPostInfo(int postID, vector<Major> majors){
    string major = majorName(majors, majorID);

    Post post;
    bool exist = false;
    for(Post p : posts){
        if(p.index == postID){
            post = p;
            exist = true;
        }
    }
    if(!exist)
        throw("Not Found");

    cout << name << " " << major << " " << semester << " ";
    for(int i=0; i<courses.size(); i++){
        cout << courses[i].courseName;
        if(i != courses.size() - 1)
            cout << ',';
    }
    cout << endl << post.index << " " << post.title << " " << post.message << endl;
}

void Student::checkNewCourse(string line, const vector<CourseSubmission>& coursesSubmission, const vector<CourseInfo>& coursesInfo){
    istringstream iss(line);
    string index , endOfLine;
    string courseID = " ";
    iss >> index >> courseID >> endOfLine;

    if(index == "id" && endOfLine == ""){
        if(isInvalidNaturalNumber(courseID))
            throw("Bad Request");

        CourseSubmission newCourse = findCourse(coursesSubmission, stoi(courseID));
        int cID  = courseId(newCourse.courseName, coursesInfo);

        checkPrerequisite(semester, newCourse.courseName, coursesInfo);
        checkMajor(majorID, cID, coursesInfo);
        checkCoursesTime(this, newCourse.time);
        checkCourseExam(this, newCourse.examDate);
        addCourse(newCourse);
        sendNotification({userID, name, "Get Course"});
    }
    else 
        throw("Bad Request");
}

void Student::deleteCourse(string line){
    istringstream iss(line);
    string index , endOfLine;
    string courseID = " ";
    iss >> index >> courseID >> endOfLine;

    if(index == "id" && endOfLine == ""){
        if(isInvalidNaturalNumber(courseID))
            throw("Bad Request");

        CourseSubmission cSDelete = findCourse(stoi(courseID), courses);
        auto it = find(courses.begin(), courses.end(), cSDelete);
        courses.erase(it);

        sendNotification({userID, name, "Delete Course"});
    }
    else 
        throw("Bad Request");
}

void Student::displayCourse(){
    if(courses.size() == 0)
        throw("Empty");
    
    for(auto c : courses)
        printCourseDetails(c);
}