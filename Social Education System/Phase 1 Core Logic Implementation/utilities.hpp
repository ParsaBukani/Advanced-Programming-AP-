#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "structurs.hpp"
using namespace std;

class User;
const string NOTHING = "";

bool isInvalidArithmeticNumber(string number);
bool isInvalidNaturalNumber(string str);
string majorName(vector<Major> majors, int majorID);
map<string, string> getData(string line, int numberOfReq, string arg1=NOTHING, string arg2=NOTHING, string arg3=NOTHING, string arg4=NOTHING, string arg5=NOTHING, string arg6=NOTHING);
string courseName(int courseID, vector<CourseInfo> coursesInfo);
int courseId(string courseName, const vector<CourseInfo>& coursesInfo);
void checkMajor(int majorID, int courseID, vector<CourseInfo> coursesInfo);
void checkCoursesTime(User* user, string time);
void checkCourseExam(User* user, string examDate);
bool haveTimeConflict(const string& time1, const string& time2);
bool haveExamTimeConflict(const string& time1, const string& time2);
void addCourseSubmission(int& index, map<string, string> data);
void checkAccess(User* SignedInUser, char mark);
void printCourseDetails(const CourseSubmission& cS);
void findAndPrintCourse(const vector<CourseSubmission>& coursesSubmission, int id);
void checkEndOfLine(string endOfLine);
CourseSubmission findCourse(const vector<CourseSubmission>& coursesSubmission, int id);
void checkPrerequisite(int semester, string courseName, const vector<CourseInfo>& coursesInfo);
CourseSubmission findCourse(int ID, vector<CourseSubmission> courses);
map<string, string> getMessageAndTitle(string line);