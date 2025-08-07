#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "utilities.hpp"
#include "algorithm"
#include "structurs.hpp"
using namespace std;

class User {
protected:
    int userID;
    string name;
    int majorID;
    string password;

    vector<User*> followings;
    vector<Post> posts;
    vector<Notification> notifications;
    vector<CourseSubmission> courses;

    int postIndex = 1;

public:
    User(int userID_, string name_, int majorID_, string password_);

    int getID(){ return userID; }
    string getPass(){ return password; }
    string getName(){ return name; }
    int getMajorID(){ return majorID; }
    vector<CourseSubmission> getCourses(){ return courses; }

    void createPost(string line);
    void deletePost(string line);

    void sendNotification(Notification n);
    void getNotification(Notification n);

    virtual void printInfo(vector<Major> majors) = 0;
    virtual void printPostInfo(int postID, vector<Major> majors) = 0;
    
    void follow(User* user);
    void acceptFollow(User* user);

    void showNotifications();

    void addCourse(CourseSubmission userCourse);
};