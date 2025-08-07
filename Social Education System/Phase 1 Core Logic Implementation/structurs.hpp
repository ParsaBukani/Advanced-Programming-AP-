#pragma once
#include <string>
#include <vector>
using namespace std;

struct Major {
    int mid;
    string name;
};

struct CourseInfo {
    int cid;
    string name;
    int credit;
    int prerequisite;
    vector<int> majorsID;
};

struct Post {
    int index;
    string title;
    string message;
    bool operator==(const Post& other) const {
        return index == other.index;
    }
};

struct Notification {
    int id;
    string sender;
    string message;
};

struct CourseSubmission {
    int id;
    string courseName;
    int capacity;
    string professor;
    string time;
    string examDate;
    int classNumber;

    bool operator==(const CourseSubmission& other) const {
        return id == other.id;
    }
};