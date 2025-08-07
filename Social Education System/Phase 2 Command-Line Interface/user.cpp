#include "user.hpp"
#include "consts.hpp"

const string TITLE = "title";
const string MESSAGE = "message";
const string IMAGE = "image";
const string PHOTO = "photo";
const vector<string> NOTIFICATIONS = {"New Post", "New Course Offering", "Get Course", "Delete Course", "New Form", "New Course Post"};


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
    map<string, string> data = getMessageAndTitle(line);

    posts.push_back({postIndex, data[TITLE], data[MESSAGE]});
    postIndex++;

    sendNotification({userID, name, NOTIFICATIONS[0]});
}

void User::deleteTheCourse(const string& courseID){
    CourseSubmission csDelete = findCourse(courses, stoi(courseID));
    auto it = find(courses.begin(), courses.end(), csDelete);
    courses.erase(it);
}

void User::deletePost(string line){
    istringstream iss(line);
    string index , endOfLine;
    string postID = " ";
    iss >> index >> postID >> endOfLine;

    if(index == ID && endOfLine == NOTHING){
        if(isInvalidNaturalNumber(postID))
            throw(BAD_REQUEST);

        for(Post post : posts){
            if(post.index == stoi(postID)){
                deleteThePostOnly(post);
                return;
            }
        }
        throw(NOT_FOUND);
    }
    else 
        throw(BAD_REQUEST);
}

void User::deleteThePostOnly(Post post){
    auto it = find(posts.begin(), posts.end(), post);
    posts.erase(it);
}

void User::follow(User* user){
    for(auto& following : followings){
        if(following->getID() == user->getID())
            throw(BAD_REQUEST);
    }
    acceptFollow(user);
    user->acceptFollow(this);
}

void User::acceptFollow(User* user){
    followings.push_back(user);
}

void User::showNotifications(){
    if(notifications.size() == 0){
        cout << EMPTY << endl;
        return;
    }
    for(int i=notifications.size()-1; i>=0; i--)
        cout << notifications[i].id << " " << notifications[i].sender << ": " << notifications[i].message << endl;

    notifications.clear();
}

void User::addCourse(CourseSubmission userCourses){
    courses.push_back(userCourses);
}

void User::setProfilePhoto(string line){
    istringstream iss(line);
    string index , endOfLine;
    string link = " ";
    iss >> index >> link >> endOfLine;

    if(index == PHOTO && endOfLine == NOTHING){
        // rest of the code (in the next Phase)
    }
    else 
        throw(BAD_REQUEST);
}

void User::createPostOnChannel(map<string, string> data, vector<CourseSubmission>& coursesSubmission){
    for(auto &cS : coursesSubmission){
        if(cS.id == stoi(data[ID])){
            checkPostPermission(cS, this);
            cS.posts.push_back({cS.postIndex++, data[TITLE], data[MESSAGE], name});
            sendNotificationToMembers(cS, userID);
            return;
        }
    }
    throw(NOT_FOUND);
}

void User::printCourses(){
    for(int i=0; i<courses.size(); i++){
        cout << courses[i].courseName;
        if(i != courses.size() - 1)
            cout << ',';
    }
}