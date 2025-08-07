#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "structurs.hpp"
using namespace std;

class User;
const string NOTHING_ = "";

bool isInvalidArithmeticNumber(string number);
bool isInvalidNaturalNumber(string str);
string majorName(vector<Major> majors, int majorID);
map<string, string> getData(string line, int numberOfReq, string arg1=NOTHING_, string arg2=NOTHING_, string arg3=NOTHING_, string arg4=NOTHING_, string arg5=NOTHING_, string arg6=NOTHING_);
bool checkPassword(User* user, string password);
string courseName(int courseID, vector<CourseInfo> coursesInfo);
int courseId(string courseName, const vector<CourseInfo>& coursesInfo);
void checkMajor(int majorID, int courseID, vector<CourseInfo> coursesInfo);
void checkCoursesTime(User* user, string time);
void checkCourseExam(User* user, string examDate);
bool haveTimeConflict(const string& time1, const string& time2);
bool haveExamTimeConflict(const string& time1, const string& time2);
void addCourseSubmission(int& index, map<string, string> data);
void checkAccess(User* SignedInUser, char mark);
void checkCourseInvolved(const string courseName, const vector<CourseSubmission>& courses);
void printCourseDetails(const CourseSubmission& cS);
void findAndPrintCourse(const vector<CourseSubmission>& coursesSubmission, int id);
void checkEndOfLine(string endOfLine);
CourseSubmission findCourse(const vector<CourseSubmission>& coursesSubmission, int id);
Post findPost(const vector<Post>& posts, int postID);
void checkPrerequisite(int semester, string courseName, const vector<CourseInfo>& coursesInfo);
map<string, string> getMessageAndTitle(string line);
map<string, string> getMessageAndCourseID(string line);
void sendNotificationToMembers(CourseSubmission& cS, int senderID);
void checkPostPermission(const CourseSubmission& cS, User* user);
User* findUser(const vector<User*>& users, int id);
int findCourseIDinPosts(const vector<Post>& posts, int formId, string& courseName);

void parseLine(string line, string& command, string& restOfLine, char& checkMark);
void parseAloneInput(string line, string& index, string& id, string& endOfLine);
void validateInput(const string& index, const string& id, const string& endOfLine);

bool printUserInfo(const vector<User*>& users, int userID, const vector<Major>& majors);
bool printUserPostInfo(const vector<User*>& users, int userID, int postID, const vector<Major>& majors);