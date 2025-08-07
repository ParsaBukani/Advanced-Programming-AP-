#pragma once
#include "user.hpp"

class Student : public User {
protected:
    int semester;

public:
    Student(int sem, int userID_, string name_, int majorID_, string password_);

    int getSemester(){ return semester; }

    void printInfo(const vector<Major>& majors);
    void printPostInfo(int postID, const vector<Major>& majors);

    void checkValidation(CourseSubmission newCourse, int cID, const vector<CourseInfo>& coursesInfo);
    void checkNewCourse(string line, vector<CourseSubmission>& coursesSubmission, const vector<CourseInfo>& coursesInfo);
    void joinCourse(vector<CourseSubmission>& coursesSubmission, int courseID);
    void deleteCourse(string line);
    void displayCourse();

    void requestTAform(string line, const vector<User*>& professors, vector<CourseSubmission>& coursesSubmission, const vector<CourseInfo>& coursesInfo);

};