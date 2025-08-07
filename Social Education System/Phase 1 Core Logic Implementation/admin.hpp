#pragma once
#include "user.hpp"
// #include "utilities.hpp"

class Professor;

class Admin : public User {
private:

public:
    Admin(int userID_ = 0, string name_ = "UT_account", int majorID_ = 0, string password_ = "UT_account");
    void printInfo(vector<Major> majors);
    void printPostInfo(int postID, vector<Major> majors);
    void offerCourse(map<string, string>& data, Professor* professor, vector<CourseInfo>& coursesInfo, int courseSubIndex);

};