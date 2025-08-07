#pragma once
#include "user.hpp"

class Professor : public User {
protected:
    string position;

public:
    Professor(string pos, int userID_, string name_, int majorID_, string password_);

    void printInfo(vector<Major> majors);
    void printPostInfo(int postID, vector<Major> majors);

    void checkNewCourse(map<string, string> data, vector<CourseInfo>& coursesInfo, int courseSubIndex);
};