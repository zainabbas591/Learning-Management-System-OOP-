#ifndef ADMINPORTAL_H
#define ADMINPORTAL_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QComboBox>
#include <QInputDialog>
#include <QMessageBox>
#include <QGroupBox> // Ye zaroori hai
#include <string>    // [FIX] Ye missing tha
#include "FileManager.h"

using namespace std; // [FIX] string use karne ke liye zaroori hai

class AdminPortal : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor mein 'string id' hai, to .cpp mein bhi hona chahiye
    explicit AdminPortal(string id, QString name, QWidget *parent = nullptr);

private slots:
    void showAddCoursePage();
    void showManageUsersPage();
    void showLogsPage();
    void showAttendancePage();
    void showGradesPage();
    void changePassword();
    void logout();

    void saveCourse();
    void addUser();
    void removeUser();
    void refreshLogs();
    void refreshAttendance();
    void refreshGrades();

private:
    string myId;
    QString adminName;
    FileManager fm;

    QWidget *centralWidget;
    QStackedWidget *pagesWidget;

    // Course Inputs
    QLineEdit *courseCodeInput;
    QLineEdit *courseNameInput;

    // User Management Inputs
    QLineEdit *newUserId;
    QLineEdit *newUserName;
    QLineEdit *newUserPass;
    QComboBox *newUserRole;
    QLineEdit *removeUserId;

    QTableWidget *logsTable;
    QTableWidget *attendanceTable;
    QTableWidget *gradesTable;

    QWidget* createAddCoursePage();
    QWidget* createManageUsersPage();
    QWidget* createLogsPage();
    QWidget* createAttendancePage();
    QWidget* createGradesPage();
};

#endif // ADMINPORTAL_H
