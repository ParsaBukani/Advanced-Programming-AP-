#include "user.hpp"


const string TITLE = "title";
const string MESSAGE = "message";
vector<string> NOTIFICATIONS = {"New Post", "New Course Offering", "Get Course", "Delete Course"};


User::User(int userID_, string name_, int majorID_, string password_){
    userID = userID_;
    name = name_;
    majorID = majorID_;
    password = password_;
}

void User::sendNotification(Notification n){
    for(auto& user : followings)
        user->getNotification(n);
}

void User::getNotification(Notification n){
    notifications.push_back(n);
}
 
void User::createPost(string line){
    istringstream iss(line);

    map<string, string> data = getMessageAndTitle(line);

    posts.push_back({postIndex, data[TITLE], data[MESSAGE]});
    postIndex++;

    sendNotification({userID, name, NOTIFICATIONS[0]});
}

void User::deletePost(string line){
    istringstream iss(line);
    string index , endOfLine;
    string postID = " ";
    iss >> index >> postID >> endOfLine;

    if(index == "id" && endOfLine == ""){
        if(isInvalidNaturalNumber(postID))
            throw("Bad Request");

        for(Post post : posts){
            if(post.index == stoi(postID)){
                auto it = find(posts.begin(), posts.end(), post);
                posts.erase(it);
                return;
            }
        }
        throw("Not Found");
    }
    else 
        throw("Bad Request");
}

void User::follow(User* user){
    for(auto& following : followings){
        if(following->getID() == user->getID())
            throw("Bad Request");
    }
    acceptFollow(user);
    user->acceptFollow(this);
}

void User::acceptFollow(User* user){
    followings.push_back(user);
}

void User::showNotifications(){
    if(notifications.size() == 0){
        cout << "Empty" << endl;
        return;
    }
    for(int i=notifications.size()-1; i>=0; i--)
        cout << notifications[i].id << " " << notifications[i].sender << ": " << notifications[i].message << endl;

    notifications.clear();
}

void User::addCourse(CourseSubmission userCourses){
    courses.push_back(userCourses);
}