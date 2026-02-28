#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Student {
public:
    string indexNumber;
    string name;

    Student(string index, string n) {
        indexNumber = index;
        name = n;
    }
};

vector<Student> students;

// ====== STUDENT MANAGEMENT ========

void saveStudentsToFile() {
    ofstream file("students.csv");
    file << "IndexNumber,Name\n";
    for (auto s : students) {
        file << s.indexNumber << "," << s.name << "\n";
    }
    file.close();
}

void loadStudentsFromFile() {
    ifstream file("students.csv");
    if (!file.is_open()) return;

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string index, name;

        getline(ss, index, ',');
        getline(ss, name);

        students.push_back(Student(index, name));
    }
    file.close();
}

void registerStudent() {
    string index, name;

    cout << "Enter Index Number: ";
    cin >> index;
    cin.ignore();

    cout << "Enter Name: ";
    getline(cin, name);

    students.push_back(Student(index, name));
    saveStudentsToFile();

    cout << "Student Registered Successfully!\n";
}

void viewStudents() {
    if (students.empty()) {
        cout << "No students registered.\n";
        return;
    }

    for (auto s : students) {
        cout << "Index: " << s.indexNumber << " | Name: " << s.name << endl;
    }
}

void searchStudent() {
    string index;
    cout << "Enter Index Number to Search: ";
    cin >> index;

    for (auto s : students) {
        if (s.indexNumber == index) {
            cout << "Found: " << s.name << endl;
            return;
        }
    }

    cout << "Student not found.\n";
}

// ======== ATTENDANCE SESSION ==========

void createSession() {
    string courseCode, date;

    cout << "Enter Course Code: ";
    cin >> courseCode;

    cout << "Enter Date (YYYY_MM_DD): ";
    cin >> date;

    string filename = "session_" + courseCode + "_" + date + ".csv";

    ofstream file(filename);
    file << "Name,IndexNumber,Status\n";

    for (auto s : students) {
        file << s.indexNumber << "," << s.name << ",Absent\n";
    }

    file.close();
    cout << "Session created successfully!\n";
}

void markAttendance() {
    string filename;
    cout << "Enter session file name (e.g., session_EEE227_2026_02_24.csv): ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Session file not found.\n";
        return;
    }

    vector<string> lines;
    string line;
    getline(file, line); // header
    lines.push_back(line);

    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    for (int i = 1; i < lines.size(); i++) {
        stringstream ss(lines[i]);
        string index, name, status;

        getline(ss, index, ',');
        getline(ss, name, ',');
        getline(ss, status);

        cout << "Mark attendance for " << name << " (P=Present, L=Late, A=Absent): ";
        char choice;
        cin >> choice;

        if (choice == 'P' || choice == 'p')
            status = "Present";
        else if (choice == 'L' || choice == 'l')
            status = "Late";
        else
            status = "Absent";

        lines[i] = index + "," + name + "," + status;
    }

    ofstream outFile(filename);
    for (auto l : lines) {
        outFile << l << "\n";
    }
    outFile.close();

    cout << "Attendance Updated Successfully!\n";
}

void viewSessionReport() {
    string filename;
    cout << "Enter session file name: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "File not found.\n";
        return;
    }

    string line;
    int present = 0, late = 0, absent = 0;

    getline(file, line); // header
    cout << line << endl;

    while (getline(file, line)) {
        cout << line << endl;

        if (line.find("Present") != string::npos) present++;
        else if (line.find("Late") != string::npos) late++;
        else absent++;
    }

    file.close();

    cout << "\nSummary:\n";
    cout << "Present: " << present << endl;
    cout << "Late: " << late << endl;
    cout << "Absent: " << absent << endl;
}

// ====== MAIN MENU =======

void menu() {
    int choice;

    do {
        cout << "\n ===DIGITAL ATTENDANCE SYSTEM ==== \n";
        cout << "1. Register Student\n";
        cout << "2. View Students\n";
        cout << "3. Search Student\n";
        cout << "4. Create Session\n";
        cout << "5. Mark Attendance\n";
        cout << "6. View Session Report\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: registerStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: createSession(); break;
            case 5: markAttendance(); break;
            case 6: viewSessionReport(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice!\n";
        }

    } while (choice != 0);
}

int main() {
    loadStudentsFromFile();
    menu();
    return 0;
}