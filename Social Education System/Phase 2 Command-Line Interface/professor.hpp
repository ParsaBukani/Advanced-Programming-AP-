#pragma once
#include "user.hpp"

class Professor : public User {
protected:
    string position;

public:
    Professor(string pos, int userID_, string name_, int majorID_, string password_);

    void printInfo(const vector<Major>& majors);
    void printPostInfo(int postID, const vector<Major>& majors);

    void checkNewCourse(map<string, string> data, vector<CourseInfo>& coursesInfo, int courseSubIndex);
    void createTAform(string line, const vector<CourseSubmission>& coursesSubmission);
    void closeTAform(string line, vector<CourseSubmission>& coursesSubmission);
    void manageChannel(int courseID, vector<CourseSubmission>& coursesSubmission);
    void manageCourseChannel(CourseSubmission& cS);

    void newTArequest(User* user, int formID, vector<CourseSubmission>& coursesSubmission, int semester, const vector<CourseInfo>& coursesInfo);

    bool isTArequestLeft(const CourseSubmission& cS);
    int numOfTArequests(const CourseSubmission& cS);
    User* getApplicant(const CourseSubmission& cS);
    void acceptApplicant(CourseSubmission& cS, User* user);
    void rejectApplicant(CourseSubmission& cS, User* user);
    void deleteTAform(int formID);

};