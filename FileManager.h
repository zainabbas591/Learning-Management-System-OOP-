#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <QDir>
#include <QDate>

using namespace std;

class FileManager {
public:
    inline FileManager() {
        if (!QDir("data").exists()) QDir().mkdir("data");
        if (!QDir("logs").exists()) QDir().mkdir("logs");
    }

    inline vector<string> parseLine(string line) {
        vector<string> result;
        stringstream s_stream(line);
        string item;
        while (getline(s_stream, item, ',')) {
            result.push_back(item);
        }
        return result;
    }

    inline void logActivity(string message) {
        ofstream file("logs/activity_log.txt", ios::app);
        if(file.is_open()) { file << message << endl; file.close(); }
    }

    // --- 1. USER MANAGEMENT (Add & Remove) ---
    inline string registerUser(string id, string name, string pass, string role) {
        // Pehle check karein ke ID exist to nahi karti
        ifstream check("data/users.txt");
        string line;
        while(getline(check, line)) {
            vector<string> row = parseLine(line);
            if(row.size() > 0 && row[0] == id) return "Error: User ID already exists!";
        }

        ofstream file("data/users.txt", ios::app);
        file << id << "," << name << "," << pass << "," << role << endl;
        file.close();
        logActivity("Admin Added User: " + name + " (" + role + ")");
        return "Success";
    }

    inline string removeUser(string targetId) {
        ifstream file("data/users.txt");
        ofstream temp("data/temp.txt");
        string line; bool found = false;
        while (getline(file, line)) {
            vector<string> row = parseLine(line);
            if (row.size() > 0 && row[0] == targetId) {
                found = true; // Is user ko skip karein (Delete)
            } else {
                temp << line << endl; // Baaki sab ko copy karein
            }
        }
        file.close(); temp.close();
        remove("data/users.txt"); rename("data/temp.txt", "data/users.txt");

        if(found) {
            logActivity("Admin Removed User ID: " + targetId);
            return "Success";
        }
        return "Error: User ID not found.";
    }

    inline string checkLogin(string inputId, string inputPass, string inputRole) {
        ifstream file("data/users.txt");
        if (!file) return "FAIL";
        string line;
        while (getline(file, line)) {
            vector<string> row = parseLine(line);
            if (row.size() >= 4 && row[0] == inputId && row[2] == inputPass && row[3] == inputRole) {
                logActivity("Login: " + row[1] + " (" + row[3] + ")");
                return row[3] + "," + row[1];
            }
        }
        return "FAIL";
    }

    // --- 2. ASSIGNMENTS (With Deadline) ---
    // Format: Course, Title, Deadline
    inline void addAssignment(string course, string title, string deadline) {
        ofstream file("data/assignments.txt", ios::app);
        file << course << "," << title << "," << deadline << endl;
        file.close();
        logActivity("Assignment Posted: " + title + " (Due: " + deadline + ")");
    }

    inline vector<vector<string>> getAssignmentsData() {
        vector<vector<string>> data;
        ifstream file("data/assignments.txt");
        string line;
        while (getline(file, line)) {
            vector<string> row = parseLine(line);
            // row[0]=Course, row[1]=Title, row[2]=Deadline
            if(row.size() >= 3) data.push_back({row[0], row[1], row[2]});
        }
        return data;
    }

    // --- 3. ATTENDANCE (Real Date) ---
    inline void markAttendance(string course, string studentId, string status) {
        ofstream file("data/attendance.txt", ios::app);
        // [NEW] "Today" ki jagah QDate::currentDate use karein
        string today = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
        file << course << "," << studentId << "," << status << "," << today << endl;
        file.close();
        logActivity("Attendance: " + studentId + " in " + course + " [" + status + "]");
    }

    inline vector<vector<string>> getStudentAttendance(string myId) {
        vector<vector<string>> data; ifstream file("data/attendance.txt"); string line;
        while (getline(file, line)) {
            vector<string> row = parseLine(line);
            if (row.size() >= 4 && row[1] == myId) data.push_back({row[0], row[2], row[3]});
        }
        return data;
    }

    inline vector<vector<string>> getAllAttendanceData() {
        vector<vector<string>> data; ifstream file("data/attendance.txt"); string line;
        while (getline(file, line)) {
            vector<string> row = parseLine(line);
            if(row.size() >= 4) data.push_back({row[0], row[1], row[2], row[3]});
        }
        return data;
    }

    // --- 4. GRADES & COURSES (Standard) ---
    inline void addCourse(string code, string name) {
        ofstream file("data/courses.txt", ios::app);
        file << code << "," << name << endl; file.close();
    }
    inline vector<vector<string>> getCoursesData() {
        vector<vector<string>> data; ifstream file("data/courses.txt"); string line;
        while (getline(file, line)) {
            vector<string> row = parseLine(line);
            if(row.size() >= 2) data.push_back({row[0], row[1]});
        }
        return data;
    }
    inline void addGrade(string course, string studentId, double marks) {
        ofstream file("data/grades.txt", ios::app);
        file << course << "," << studentId << "," << marks << endl; file.close();
    }
    inline vector<vector<string>> getAllGradesData() {
        vector<vector<string>> data; ifstream file("data/grades.txt"); string line;
        while (getline(file, line)) {
            vector<string> row = parseLine(line);
            if(row.size() >= 3) data.push_back({row[0], row[1], row[2]});
        }
        return data;
    }
    inline vector<vector<string>> getTranscriptData(string myId) {
        vector<vector<string>> data; ifstream file("data/grades.txt"); string line;
        while (getline(file, line)) {
            vector<string> row = parseLine(line);
            if (row.size() >= 3 && row[1] == myId) data.push_back({row[0], row[2]});
        }
        return data;
    }

    // --- 5. LOGS & PASSWORD (Same as before) ---
    inline vector<string> getLogsData() {
        vector<string> data; ifstream file("logs/activity_log.txt"); string line;
        while(getline(file, line)) if(!line.empty()) data.push_back(line);
        if(data.empty()) data.push_back("No activity recorded yet.");
        return data;
    }
    inline string changePassword(string myId, string newPass) {
        ifstream file("data/users.txt"); ofstream temp("data/temp.txt");
        string line; bool found = false;
        while (getline(file, line)) {
            vector<string> row = parseLine(line);
            if (row.size() >= 4) {
                if (row[0] == myId) { row[2] = newPass; found = true; }
                temp << row[0] << "," << row[1] << "," << row[2] << "," << row[3] << endl;
            }
        }
        file.close(); temp.close(); remove("data/users.txt"); rename("data/temp.txt", "data/users.txt");
        if(found) logActivity("Password Changed for ID: " + myId);
        return found ? "Success" : "Error";
    }

    // --- [NEW] 6. SUBMISSIONS (Assignment Answer) ---
    inline void saveSubmission(string course, string title, string studentId, string answer) {
        ofstream file("data/submissions.txt", ios::app);

        // Comma hata dein taake CSV format kharab na ho (Comma ko space bana dein)
        for(char &c : answer) if(c == ',') c = ' ';

        string date = QDate::currentDate().toString("yyyy-MM-dd").toStdString();
        // Format: Course, Title, ID, Date, Answer
        file << course << "," << title << "," << studentId << "," << date << "," << answer << endl;
        file.close();
        logActivity("Student " + studentId + " submitted: " + title);
    }

    inline bool isSubmitted(string course, string title, string studentId) {
        ifstream file("data/submissions.txt");
        string line;
        while(getline(file, line)) {
            vector<string> row = parseLine(line);
            // row[0]=Course, row[1]=Title, row[2]=StudentID
            if(row.size() >= 3 && row[0] == course && row[1] == title && row[2] == studentId) {
                return true; // Already submitted
            }
        }
        return false;
    }
};

#endif // FILEMANAGER_H
