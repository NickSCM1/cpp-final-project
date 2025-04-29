//****************************************************** 
// Author:      Nickolas Marino
// Asignment:   Final Project
// Description: The Student Grade Analyzer
//              
//******************************************************

#include "StudentGradeAnalyzer.h"
#include <fstream>
#include <sstream>
#include <iomanip>

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void inputStudent(Student& s) {
    std::cout << "Enter student name: ";
    std::getline(std::cin, s.name);
}

void calculateClassAverage(ClassGrade& c) {
    if (c.assignments.empty()) {
        c.average = 0;
        c.letterGrade = "F";
        return;
    }
    int sum = 0;
    for (int grade : c.assignments) sum += grade;
    c.average = static_cast<float>(sum) / c.assignments.size();
    c.letterGrade = getLetterGrade(c.average);
}

std::string getLetterGrade(float avg) {
    if (avg >= 90) return "A";
    else if (avg >= 80) return "B";
    else if (avg >= 70) return "C";
    else if (avg >= 60) return "D";
    else return "F";
}

float convertToGradePoint(float average) {
    if (average >= 90) return 4.0;
    else if (average >= 80) return 3.0;
    else if (average >= 70) return 2.0;
    else if (average >= 60) return 1.0;
    else return 0.0;
}

void calculateGPA(Student& s) {
    if (s.classes.empty()) {
        s.gpa = 0;
        return;
    }
    float totalPoints = 0;
    for (auto& c : s.classes) {
        calculateClassAverage(c);
        totalPoints += convertToGradePoint(c.average);
    }
    s.gpa = totalPoints / s.classes.size();
}

void displayAllStudents(const std::vector<Student>& students) {
    std::cout << "\n--- Students and GPA ---\n";
    for (size_t i = 0; i < students.size(); i++) {
        std::string color = (students[i].gpa >= 3.0) ? GREEN :
                            (students[i].gpa >= 1.0) ? YELLOW : RED;
        std::cout << i + 1 << ". " << students[i].name
                  << " | GPA: " << color << std::fixed << std::setprecision(2)
                  << students[i].gpa << RESET << "\n";
    }
    std::cout << "\n";
}

void manageStudent(Student& student, std::vector<Student>& students, const std::string& filename) {
    int choice;
    do {
        std::cout << "\nManaging student: " << student.name << "\n";
        std::cout << "0. Save & Return to Main Menu\n";
        std::cout << "1. View/Edit Classes\n";
        std::cout << "2. Add Class\n";
        std::cout << "3. Remove Class\n";
        std::cout << "Select: ";
        std::cin >> choice;
        if (std::cin.fail()) { clearInput(); std::cout << RED << "Invalid input! Try again.\n" << RESET; continue; }
        std::cin.ignore();

        if (choice == 0) {
            saveToFile(students, filename);
            std::cout << GREEN << "Saved and returned to main menu.\n" << RESET;
            break;
        }

        if (choice == 1) {
            for (size_t i = 0; i < student.classes.size(); ++i) {
                const auto& c = student.classes[i];
                std::string color = (c.average >= 70) ? GREEN :
                                    (c.average >= 60) ? YELLOW : RED;
                std::cout << i + 1 << ". " << c.className
                          << " | Avg: " << color << std::fixed << std::setprecision(2)
                          << c.average << " (" << c.letterGrade << ")" << RESET << "\n";
            }
            int classIndex;
            std::cout << "Select class to edit (0 to cancel): ";
            std::cin >> classIndex;
            if (std::cin.fail()) { clearInput(); std::cout << RED << "Invalid input!\n" << RESET; continue; }
            std::cin.ignore();

            if (classIndex == 0) continue;
            if (classIndex > 0 && classIndex <= (int)student.classes.size()) {
                ClassGrade& c = student.classes[classIndex - 1];
                int classOption;
                do {
                    std::cout << "\nClass: " << c.className << "\n";
                    std::cout << "Assignments: ";
                    for (int g : c.assignments) std::cout << g << " ";
                    std::string color = (c.average >= 70) ? GREEN :
                                        (c.average >= 60) ? YELLOW : RED;
                    std::cout << "\nAverage: " << color << c.average << " (" << c.letterGrade << ")" << RESET << "\n";
                    std::cout << "0. Save & Return to Main Menu\n1. Add Grade\n2. Remove Grade\n3. Back\nChoice: ";
                    std::cin >> classOption;
                    if (std::cin.fail()) { clearInput(); std::cout << RED << "Invalid input!\n" << RESET; continue; }

                    if (classOption == 0) {
                        saveToFile(students, filename);
                        std::cout << GREEN << "Saved and returned to main menu.\n" << RESET;
                        return;
                    }
                    else if (classOption == 1) {
                        int grade;
                        std::cout << "Enter grade: ";
                        std::cin >> grade;
                        if (std::cin.fail()) { clearInput(); std::cout << RED << "Invalid grade!\n" << RESET; continue; }
                        c.assignments.push_back(grade);
                    }
                    else if (classOption == 2 && !c.assignments.empty()) {
                        int removeIndex;
                        std::cout << "Enter grade index to remove (1-" << c.assignments.size() << "): ";
                        std::cin >> removeIndex;
                        if (std::cin.fail() || removeIndex < 1 || removeIndex > (int)c.assignments.size()) {
                            clearInput();
                            std::cout << RED << "Invalid index!\n" << RESET;
                            continue;
                        }
                        c.assignments.erase(c.assignments.begin() + removeIndex - 1);
                    }
                    calculateClassAverage(c);
                    calculateGPA(student);
                } while (classOption != 3);
            }
        } else if (choice == 2) {
            ClassGrade newClass;
            std::cout << "Enter class name: ";
            std::getline(std::cin, newClass.className);
            student.classes.push_back(newClass);
        } else if (choice == 3) {
            for (size_t i = 0; i < student.classes.size(); ++i) {
                std::cout << i + 1 << ". " << student.classes[i].className << "\n";
            }
            int removeIndex;
            std::cout << "Select class to remove: ";
            std::cin >> removeIndex;
            if (std::cin.fail() || removeIndex < 1 || removeIndex > (int)student.classes.size()) {
                clearInput();
                std::cout << RED << "Invalid selection!\n" << RESET;
                continue;
            }
            student.classes.erase(student.classes.begin() + removeIndex - 1);
            calculateGPA(student);
        } else {
            std::cout << RED << "Invalid option!\n" << RESET;
        }
    } while (true);
}

bool saveToFile(const std::vector<Student>& students, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) return false;

    for (const auto& student : students) {
        file << student.name << "\n";
        file << student.classes.size() << "\n";
        for (const auto& c : student.classes) {
            file << c.className << "\n";
            file << c.assignments.size() << "\n";
            for (int g : c.assignments) file << g << " ";
            file << "\n";
        }
    }
    return true;
}

bool loadFromFile(std::vector<Student>& students, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return false;

    students.clear();
    std::string line;
    while (std::getline(file, line)) {
        Student student;
        student.name = line;

        size_t classCount;
        file >> classCount;
        file.ignore();

        for (size_t i = 0; i < classCount; ++i) {
            ClassGrade c;
            std::getline(file, c.className);

            size_t assignmentCount;
            file >> assignmentCount;
            file.ignore();

            c.assignments.clear();
            std::getline(file, line);
            std::istringstream ss(line);
            for (size_t j = 0; j < assignmentCount; ++j) {
                int grade;
                ss >> grade;
                c.assignments.push_back(grade);
            }

            calculateClassAverage(c);
            student.classes.push_back(c);
        }

        calculateGPA(student);
        students.push_back(student);
    }
    return true;
}

bool deleteStudent(std::vector<Student>& students, const std::string& filename) {
    if (students.empty()) {
        std::cout << RED << "No students to delete.\n" << RESET;
        return false;
    }

    displayAllStudents(students);
    int idx;
    std::cout << "Enter the number of the student to delete (0 to cancel): ";
    std::cin >> idx;

    if (std::cin.fail() || idx < 0 || idx > (int)students.size()) {
        clearInput();
        std::cout << RED << "Invalid input.\n" << RESET;
        return false;
    }

    if (idx == 0) return false;

    std::string name = students[idx - 1].name;
    students.erase(students.begin() + idx - 1);
    saveToFile(students, filename);
    std::cout << GREEN << "Student '" << name << "' deleted and saved.\n" << RESET;
    return true;
}

int main() {
    std::vector<Student> students;
    std::string filename = "students.txt";

    loadFromFile(students, filename);

    int choice;
    do {
        std::cout << "\n--- Student Grade Analyzer ---\n";
        std::cout << "1. Add Student\n2. Display All Students\n3. Manage Student\n4. Delete Student\n5. Save and Exit\nChoice: ";
        std::cin >> choice;
        std::cin.ignore();

        if (std::cin.fail()) {
            clearInput();
            std::cout << RED << "Invalid input! Try again.\n" << RESET;
            continue;
        }

        switch (choice) {
            case 1: {
                Student newStudent;
                inputStudent(newStudent);
                students.push_back(newStudent);
                break;
            }
            case 2:
                displayAllStudents(students);
                break;
            case 3:
                displayAllStudents(students);
                if (!students.empty()) {
                    int idx;
                    std::cout << "Select student to manage (0 to return): ";
                    std::cin >> idx;
                    std::cin.ignore();
                    if (idx == 0) break;
                    if (idx > 0 && idx <= (int)students.size()) {
                        manageStudent(students[idx - 1], students, filename);
                    } else {
                        std::cout << RED << "Invalid selection.\n" << RESET;
                    }
                }
                break;
            case 4:
                deleteStudent(students, filename);
                break;
            case 5:
                saveToFile(students, filename);
                std::cout << GREEN << "Data saved. Exiting...\n" << RESET;
                break;
            default:
                std::cout << RED << "Invalid option!\n" << RESET;
        }
    } while (choice != 5);

    return 0;
}
