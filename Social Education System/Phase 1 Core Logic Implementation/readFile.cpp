#include "readFile.hpp"

string tokenize(char index, int placing, string line)
{
    line += index;
    int counter = 1;
    string substring = "";
    for(auto x : line){
        if(x==index)
        {
            if(counter==placing)
                return substring;
            else{
                counter++;
                substring = "";
            }
        }
        else
            substring = substring + x;
    }
    return "";
}

void readMajors(const string fileName, vector<Major>& majors)
{
    ifstream majorsFile(fileName);
    string line;
    getline(majorsFile, line);
    while(getline(majorsFile, line))
    {
        Major m;
        m.mid = stoi(tokenize(',', 1, line));
        m.name = tokenize(',', 2, line);
        majors.push_back(m);
    }
    majorsFile.close();
}

void readCourses(const string fileName, vector<CourseInfo>& courses)
{
    ifstream courseFile(fileName);
    string line;

    getline(courseFile, line);
    while(getline(courseFile, line))
    {
        CourseInfo c;
        c.cid = stoi(tokenize(',', 1, line));
        c.name = tokenize(',', 2, line);
        c.credit = stoi(tokenize(',', 3, line));
        c.prerequisite = stoi(tokenize(',', 4, line));

        string majors = tokenize(',', 5, line);
        int i=1;
        while (tokenize(';', i, majors) != "")
        {
            c.majorsID.push_back(stoi(tokenize(';', i, majors)));
            i++;
        }

        courses.push_back(c);
    }
    courseFile.close();
}

void readStudents(const string fileName, vector<User*>& students)
{
    ifstream studentFile(fileName);
    string line;

    getline(studentFile, line);
    while(getline(studentFile, line))
    {
        int id_ = stoi(tokenize(',', 1, line));
        string name_ = tokenize(',', 2, line);
        int majorID_ = stoi(tokenize(',', 3, line));
        int semester_ = stoi(tokenize(',', 4, line));
        string password_ = tokenize(',', 5, line);

        students.push_back(new Student(semester_, id_, name_, majorID_, password_));
    }
    studentFile.close();
}

void readProfessors(const string fileName, vector<User*>& professors)
{
    ifstream professorFile(fileName);
    string line;

    getline(professorFile, line);
    while(getline(professorFile, line))
    {
        int id_ = stoi(tokenize(',', 1, line));
        string name_ = tokenize(',', 2, line);
        int majorID_ = stoi(tokenize(',', 3, line));
        string position = tokenize(',', 4, line);
        string password_ = tokenize(',', 5, line);

        professors.push_back(new Professor(position, id_, name_, majorID_, password_));
    }
    professorFile.close();
}

void readFromFile(char *argv[], vector<Major>& majors, vector<CourseInfo>& courses, vector<User*>& students, vector<User*>& professors){
    const string MAJORS = argv[1];
    const string STUDENTS = argv[2];
    const string COURSES = argv[3];
    const string PROFESSORS = argv[4];

    readMajors(MAJORS, majors);
    readCourses(COURSES, courses);
    readStudents(STUDENTS, students);
    readProfessors(PROFESSORS, professors);
}