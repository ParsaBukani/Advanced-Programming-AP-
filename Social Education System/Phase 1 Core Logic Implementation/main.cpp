#include "readFile.hpp"
#include "system.hpp"

int main(int argc, char *argv[])
{
    vector<User*> students;
    vector<User*> professors;
    vector<CourseInfo> courses;
    vector<Major> majors;

    readFromFile(argv, majors, courses, students, professors);
    
    System system(students, professors, courses, majors);
    system.run();
}