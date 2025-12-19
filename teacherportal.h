#ifndef TEACHERPORTAL_H
#define TEACHERPORTAL_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QDateEdit> // Assignment Date ke liye
#include <string>    // [FIX] Ye missing tha
#include "FileManager.h"

using namespace std; // [FIX] Ye bhi missing tha

class TeacherPortal : public QMainWindow
{
    Q_OBJECT

public:
    // Ab compiler ko "string" ka pata hai
    explicit TeacherPortal(string id, QString name, QWidget *parent = nullptr);

private slots:
    void showAttendancePage();
    void showGradesPage();
    void showAssignmentPage();
    void changePassword();
    void logout();
    void saveAttendance();
    void saveGrade();
    void saveAssignment();

private:
    string myId;
    QString teacherName;
    FileManager fm;

    QWidget *centralWidget;
    QStackedWidget *pagesWidget;

    QLineEdit *attCourseInput;
    QLineEdit *attStudentIdInput;
    QComboBox *attStatusBox;

    QLineEdit *gradeCourseInput;
    QLineEdit *gradeStudentIdInput;
    QLineEdit *gradeMarksInput;

    QLineEdit *assignCourseInput;
    QLineEdit *assignTitleInput;
    QDateEdit *assignDeadlineInput;

    QWidget* createAttendancePage();
    QWidget* createGradesPage();
    QWidget* createAssignmentPage();
};

#endif // TEACHERPORTAL_H
