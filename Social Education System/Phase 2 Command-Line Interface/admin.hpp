#pragma once
#include "user.hpp"

const string UT_ACCOUNT = "UT_account";
class Professor;

class Admin : public User {
private:

public:
    Admin(int userID_ = 0, string name_ = UT_ACCOUNT, int majorID_ = 0, string password_ = UT_ACCOUNT);
    void printInfo(const vector<Major>& majors);
    void printPostInfo(int postID, const vector<Major>& majors);
    void offerCourse(map<string, string>& data, Professor* professor, vector<CourseInfo>& coursesInfo, int courseSubIndex);

};