#pragma once
#include "user.hpp"
#include "structurs.hpp"
#include "student.hpp"
#include "professor.hpp"
#include "admin.hpp"

class System {
private:
    Admin admin;
    vector<User*> students;
    vector<User*> professors;

    vector<CourseInfo> coursesInfo;
    vector<Major> majors;

    vector<CourseSubmission> coursesSubmission;
    int courseSubIndex = 1;

    User* SignedInUser = nullptr;

public:
    System(vector<User*> s_, vector<User*> p_, vector<CourseInfo> c_, vector<Major> m_);
    void run();

    void Get(string line);
    void Post(string line);
    void Put(string line);
    void Delete(string line);

    void logIn(string line);
    void checkLogIn(map<string, string> data);
    void logOut();

    void getPersonalPageInfo(string line);
    void getPostInfo(string line);

    void connect(string line);
    bool follow(vector<User*> users, int followingID);

    void courseOffer(string line);
    void addCourseSub(map<string, string> data, User* teacher);

    void displayCourses(string line);
    void displayCourseChannel(string line);
    void displayPostChannel(string line);
    void printCourseChannel(const CourseSubmission& cS);
    void printPostChannel(const CourseSubmission& cS, int postID);
    
    void coursePost(string line);
};