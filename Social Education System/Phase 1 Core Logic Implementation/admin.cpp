#include "admin.hpp"
#include "professor.hpp"

vector<string> NOTIFICATION = {"New Post", "New Course Offering", "Get Course", "Delete Course"};

Admin::Admin(int userID_, string name_, int majorID_, string password_) : 
User(userID_, name_, majorID, password_) {}

void Admin::printInfo(vector<Major> majors){
    cout << "UT_account" << endl;
    for(int i=0; i<posts.size(); i++)
        cout << posts[i].index << " " << posts[i].title << endl;
}

void Admin::printPostInfo(int postID, vector<Major> majors){
    for(Post post : posts){
        if(post.index == postID){
           cout << "UT_account" << endl << post.index << " " << post.title << " " << post.message << endl;
           return;
        }
    }
    throw("Not Found");
}

void Admin::offerCourse(map<string, string>& data, Professor* professor, vector<CourseInfo>& coursesInfo, int courseSubIndex){
    professor->checkNewCourse(data, coursesInfo, courseSubIndex);
    sendNotification({professor->getID(), professor->getName(), NOTIFICATION[1]});
}