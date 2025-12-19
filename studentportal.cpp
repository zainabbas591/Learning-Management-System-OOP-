#include "studentportal.h"
#include "mainwindow.h"
#include <QDate> // Date logic ke liye

// [FIX] Constructor ID aur Name le raha hai
StudentPortal::StudentPortal(string id, string name, QWidget *parent)
    : QMainWindow(parent), myId(id), myName(name)
{
    setWindowTitle("Student Portal");
    resize(900, 600);
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *contentLayout = new QHBoxLayout();

    welcomeLabel = new QLabel("Welcome, " + QString::fromStdString(myName));
    welcomeLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #2c3e50; padding: 10px;");
    welcomeLabel->setAlignment(Qt::AlignCenter);

    // Sidebar
    QVBoxLayout *menuLayout = new QVBoxLayout();

    QPushButton *btnCourses = new QPushButton("My Courses", this);
    QPushButton *btnAssign = new QPushButton("Assignments", this);
    QPushButton *btnAtt = new QPushButton("My Attendance", this);
    QPushButton *btnGrades = new QPushButton("Transcript", this);
    QPushButton *btnPass = new QPushButton("Change Password", this);
    QPushButton *btnLogout = new QPushButton("Logout", this);
    btnLogout->setStyleSheet("background-color: #e74c3c; color: white;");

    QString btnStyle = "QPushButton { padding: 15px; text-align: left; background-color: #34495e; color: white; border-radius: 5px; } QPushButton:hover { background-color: #2c3e50; }";
    btnCourses->setStyleSheet(btnStyle);
    btnAssign->setStyleSheet(btnStyle);
    btnAtt->setStyleSheet(btnStyle);
    btnGrades->setStyleSheet(btnStyle);
    btnPass->setStyleSheet(btnStyle);

    menuLayout->addWidget(btnCourses);
    menuLayout->addWidget(btnAssign);
    menuLayout->addWidget(btnAtt);
    menuLayout->addWidget(btnGrades);
    menuLayout->addWidget(btnPass);
    menuLayout->addStretch();
    menuLayout->addWidget(btnLogout);

    // Right Side: Table aur Submit Button
    QVBoxLayout *rightLayout = new QVBoxLayout();

    table = new QTableWidget(this);
    table->setStyleSheet("QTableWidget { font-size: 14px; gridline-color: #bdc3c7; selection-background-color: #3498db; } QHeaderView::section { background-color: #ecf0f1; padding: 5px; font-weight: bold; border: 1px solid #bdc3c7; }");
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Submit Button (Start mein hidden hoga)
    btnSubmitAction = new QPushButton("Submit Selected Assignment Solution");
    btnSubmitAction->setStyleSheet("background-color: #27ae60; color: white; padding: 12px; font-weight: bold;");
    btnSubmitAction->setVisible(false);

    rightLayout->addWidget(table);
    rightLayout->addWidget(btnSubmitAction);

    contentLayout->addLayout(menuLayout, 1);
    contentLayout->addLayout(rightLayout, 4);

    mainLayout->addWidget(welcomeLabel);
    mainLayout->addLayout(contentLayout);

    connect(btnCourses, &QPushButton::clicked, this, &StudentPortal::viewCourses);
    connect(btnAssign, &QPushButton::clicked, this, &StudentPortal::viewAssignments);
    connect(btnAtt, &QPushButton::clicked, this, &StudentPortal::viewAttendance);
    connect(btnGrades, &QPushButton::clicked, this, &StudentPortal::viewTranscript);
    connect(btnPass, &QPushButton::clicked, this, &StudentPortal::changePassword);
    connect(btnLogout, &QPushButton::clicked, this, &StudentPortal::logout);

    // Button Connect
    connect(btnSubmitAction, &QPushButton::clicked, this, &StudentPortal::submitSolution);

    viewCourses();
}

StudentPortal::~StudentPortal() {}

void StudentPortal::viewCourses() {
    btnSubmitAction->setVisible(false);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"Course Code", "Course Name"});
    vector<vector<string>> data = fm.getCoursesData();
    table->setRowCount(data.size());
    for(int i=0; i < data.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(data[i][0])));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(data[i][1])));
    }
}

void StudentPortal::viewAssignments() {
    btnSubmitAction->setVisible(true); // Assignments mein button show hoga
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Course", "Assignment Title", "Deadline", "Status"});

    vector<vector<string>> data = fm.getAssignmentsData();
    table->setRowCount(data.size());
    QDate today = QDate::currentDate();

    for(int i=0; i < data.size(); i++) {
        string course = data[i][0];
        string title = data[i][1];
        string deadlineStr = data[i][2];

        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(course)));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(title)));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(deadlineStr)));

        QTableWidgetItem *statusItem;
        QDate deadline = QDate::fromString(QString::fromStdString(deadlineStr), "yyyy-MM-dd");

        // Check agar submit ho chuka hai
        if (fm.isSubmitted(course, title, myId)) {
            statusItem = new QTableWidgetItem("Submitted");
            statusItem->setForeground(Qt::darkGreen);
            statusItem->setBackground(QColor("#d4edda"));
        }
        // Check agar Late hai
        else if (today > deadline) {
            statusItem = new QTableWidgetItem("Deadline Exceeded");
            statusItem->setForeground(Qt::red);
            statusItem->setBackground(QColor("#f8d7da"));
        }
        else {
            statusItem = new QTableWidgetItem("Pending");
            statusItem->setForeground(Qt::blue);
        }
        table->setItem(i, 3, statusItem);
    }
}

void StudentPortal::submitSolution() {
    int row = table->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "Select", "Please select an assignment from the table first!");
        return;
    }

    string course = table->item(row, 0)->text().toStdString();
    string title = table->item(row, 1)->text().toStdString();
    string status = table->item(row, 3)->text().toStdString();

    if (status == "Submitted") {
        QMessageBox::information(this, "Info", "You have already submitted this assignment.");
        return;
    }
    if (status == "Deadline Exceeded") {
        QMessageBox::critical(this, "Late", "Sorry, the deadline has passed.");
        return;
    }

    bool ok;
    QString answer = QInputDialog::getMultiLineText(this, "Submit Solution",
                                                    "Write your answer for " + QString::fromStdString(title) + ":", "", &ok);

    if (ok && !answer.isEmpty()) {
        fm.saveSubmission(course, title, myId, answer.toStdString());
        QMessageBox::information(this, "Success", "Assignment Submitted Successfully!");
        viewAssignments(); // Refresh Table
    }
}

void StudentPortal::viewAttendance() {
    btnSubmitAction->setVisible(false);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Course", "Status", "Date"});
    vector<vector<string>> data = fm.getStudentAttendance(myId);
    table->setRowCount(data.size());
    for(int i=0; i < data.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(data[i][0])));
        QTableWidgetItem *statusItem = new QTableWidgetItem(QString::fromStdString(data[i][1]));
        if(data[i][1] == "Absent") statusItem->setForeground(Qt::red);
        else statusItem->setForeground(Qt::darkGreen);
        table->setItem(i, 1, statusItem);
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(data[i][2])));
    }
}

void StudentPortal::viewTranscript() {
    btnSubmitAction->setVisible(false);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Course", "Marks", "GPA"});
    vector<vector<string>> data = fm.getTranscriptData(myId);
    table->setRowCount(data.size());
    for(int i=0; i < data.size(); i++) {
        QString course = QString::fromStdString(data[i][0]);
        double marks = 0.0;
        try { marks = stod(data[i][1]); } catch (...) { marks = 0.0; }

        double gpa = 0.0;
        if (marks >= 85) gpa = 4.0;
        else if (marks >= 80) gpa = 3.7;
        else if (marks >= 70) gpa = 3.0;
        else if (marks >= 60) gpa = 2.5;
        else if (marks >= 50) gpa = 2.0;
        else gpa = 1.0;

        table->setItem(i, 0, new QTableWidgetItem(course));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(marks)));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(gpa)));
    }
}

void StudentPortal::changePassword() {
    bool ok;
    QString newPass = QInputDialog::getText(this, "Security", "Enter New Password:", QLineEdit::Password, "", &ok);
    if (ok && !newPass.isEmpty()) {
        QMessageBox::information(this, "Status", QString::fromStdString(fm.changePassword(myId, newPass.toStdString())));
    }
}

void StudentPortal::logout() {
    this->close();
    MainWindow *login = new MainWindow();
    login->show();
}
