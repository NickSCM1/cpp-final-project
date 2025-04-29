#ifndef STUDENT_GRADE_ANALYZER_H
#define STUDENT_GRADE_ANALYZER_H

#include <iostream>
#include <vector>
#include <string>
#include <limits>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

struct ClassGrade {
    std::string className;
    std::vector<int> assignments;
    float average = 0.0;
    std::string letterGrade;
};

struct Student {
    std::string name;
    std::vector<ClassGrade> classes;
    float gpa = 0.0;
};

void clearInput();
void inputStudent(Student& s);
void calculateClassAverage(ClassGrade& c);
std::string getLetterGrade(float avg);
float convertToGradePoint(float average);
void calculateGPA(Student& s);
void displayAllStudents(const std::vector<Student>& students);
void manageStudent(Student& student, std::vector<Student>& students, const std::string& filename);
bool saveToFile(const std::vector<Student>& students, const std::string& filename);
bool loadFromFile(std::vector<Student>& students, const std::string& filename);
bool deleteStudent(std::vector<Student>& students, const std::string& filename);

#endif 