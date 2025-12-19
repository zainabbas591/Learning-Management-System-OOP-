#ifndef STUDENTPORTAL_H
#define STUDENTPORTAL_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include "FileManager.h"

class StudentPortal : public QMainWindow
{
    Q_OBJECT

public:
    StudentPortal(string id, string name, QWidget *parent = nullptr);
    ~StudentPortal();

private slots:
    void viewCourses();
    void viewAssignments();
    void submitSolution(); // [NEW] Button click par chalega
    void viewTranscript();
    void viewAttendance();
    void changePassword();
    void logout();

private:
    string myId;
    string myName;
    FileManager fm;

    QWidget *centralWidget;
    QLabel *welcomeLabel;
    QTableWidget *table;
    QPushButton *btnSubmitAction; // [NEW] Table ke neechay wala button
};

#endif // STUDENTPORTAL_H
