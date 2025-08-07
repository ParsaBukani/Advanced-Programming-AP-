#pragma once
#include "user.hpp"

class Student : public User {
protected:
    int semester;

public:
    Student(int sem, int userID_, string name_, int majorID_, string password_);
    void printInfo(vector<Major> majors);
    void printPostInfo(int postID, vector<Major> majors);

    void checkNewCourse(string line, const vector<CourseSubmission>& coursesSubmission, const vector<CourseInfo>& coursesInfo);
    void deleteCourse(string line);
    void displayCourse();
};