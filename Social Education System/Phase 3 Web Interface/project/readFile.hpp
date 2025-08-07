#pragma once
#include <fstream>
#include <string>
#include <vector>
using namespace std;
#include "user.hpp"
#include "structurs.hpp"
#include "student.hpp"
#include "professor.hpp"

string tokenize(char index, int placing, string line);
void readMajors(const string fileName, vector<Major>& majors);
void readCourses(const string fileName, vector<CourseInfo>& courses);
void readStudents(const string fileName, vector<User*>& students);
void readProfessors(const string fileName, vector<User*>& professors);
void readFromFile(char *argv[], vector<Major>& majors, vector<CourseInfo>& courses, vector<User*>& students, vector<User*>& professors);