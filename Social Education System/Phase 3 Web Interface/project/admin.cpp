#include "admin.hpp"
#include "professor.hpp"
#include "consts.hpp"


Admin::Admin(int userID_, string name_, int majorID_, string password_) : 
User(userID_, name_, majorID, password_) {}

void Admin::printInfo(const vector<Major>& majors){
    cout << UT_ACCOUNT << endl;
    for(int i = posts.size()-1; i >= 0; i--)
        cout << posts[i].index << " " << posts[i].title << endl;
}

void Admin::printPostInfo(int postID, const vector<Major>& majors){
    for(Post post : posts){
        if(post.index == postID){
           cout << UT_ACCOUNT << endl << post.index << " " << post.title << " " << post.message << endl;
           return;
        }
    }
    throw(NOT_FOUND);
}

void Admin::offerCourse(map<string, string>& data, Professor* professor, vector<CourseInfo>& coursesInfo, int courseSubIndex){
    professor->checkNewCourse(data, coursesInfo, courseSubIndex);
    sendNotification({professor->getID(), professor->getName(), NOTIFICATIONS[1]});
}