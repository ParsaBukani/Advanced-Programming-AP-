#include "professor.hpp"

Professor::Professor(string pos, int userID_, string name_, int majorID_, string password_) :
User(userID_, name_, majorID_, password_) {
    position = pos;
}

void Professor::printInfo(vector<Major> majors){
    string major = majorName(majors, majorID);

    cout << name << " " << major << " " << position << " ";
    for(int i=0; i<courses.size(); i++){
        cout << courses[i].courseName;
        if(i != courses.size() - 1)
            cout << ',';
    }
    cout << endl;
    for(int i=0; i<posts.size(); i++)
        cout << posts[i].index << " " << posts[i].title << endl;
}

void Professor::printPostInfo(int postID, vector<Major> majors){
    string major = majorName(majors, majorID);

    Post post;
    bool exist = false;
    for(Post p : posts){
        if(p.index == postID){
            post = p;
            exist = true;
        }
    }
    if(!exist)
        throw("Not Found");

    cout << name << " " << major << " " << position << " ";
    for(int i=0; i<courses.size(); i++){
        cout << courses[i].courseName;
        if(i != courses.size() - 1)
            cout << ',';
    }
    cout << endl << post.index << " " << post.title << " " << post.message << endl;
}

void Professor::checkNewCourse(map<string, string> data, vector<CourseInfo>& coursesInfo, int courseSubIndex){
    string cName = courseName(stoi(data["course_id"]), coursesInfo);

    checkMajor(majorID, stoi(data["course_id"]), coursesInfo);
    checkCoursesTime(this, data["time"]);
    
    CourseSubmission cS = {courseSubIndex, cName, stoi(data["capacity"]), 
    name, data["time"], data["exam_date"], stoi(data["class_number"])};
    addCourse(cS);
}
