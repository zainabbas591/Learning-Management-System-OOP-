#include "adminportal.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QGroupBox> // [FIX] Ye zaroori hai UI ke liye

// [FIX] Constructor mein ID add ki gayi hai
AdminPortal::AdminPortal(string id, QString name, QWidget *parent)
    : QMainWindow(parent), myId(id), adminName(name)
{
    setWindowTitle("Admin Portal - " + adminName);
    resize(1000, 600);
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    QVBoxLayout *sidebarLayout = new QVBoxLayout();

    QLabel *welcome = new QLabel("ADMIN PANEL\n" + adminName);
    welcome->setStyleSheet("font-size: 18px; font-weight: bold; color: #c0392b; margin-bottom: 20px;");

    QPushButton *btnAddCourse = new QPushButton("Manage Courses");
    QPushButton *btnUsers = new QPushButton("Manage Users"); // [NEW]
    QPushButton *btnLogs = new QPushButton("View Logs");
    QPushButton *btnAttendance = new QPushButton("View Attendance");
    QPushButton *btnGrades = new QPushButton("View Grades");
    QPushButton *btnPass = new QPushButton("Change Password");
    QPushButton *btnLogout = new QPushButton("Logout");

    QString btnStyle = "QPushButton { text-align: left; padding: 15px; background-color: #34495e; color: white; border-radius: 5px; } QPushButton:hover { background-color: #2c3e50; }";
    btnAddCourse->setStyleSheet(btnStyle);
    btnUsers->setStyleSheet(btnStyle);
    btnLogs->setStyleSheet(btnStyle);
    btnAttendance->setStyleSheet(btnStyle);
    btnGrades->setStyleSheet(btnStyle);
    btnPass->setStyleSheet(btnStyle);
    btnLogout->setStyleSheet("background-color: #e74c3c; color: white; padding: 15px; border-radius: 5px;");

    sidebarLayout->addWidget(welcome);
    sidebarLayout->addWidget(btnAddCourse);
    sidebarLayout->addWidget(btnUsers);
    sidebarLayout->addWidget(btnLogs);
    sidebarLayout->addWidget(btnAttendance);
    sidebarLayout->addWidget(btnGrades);
    sidebarLayout->addWidget(btnPass);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(btnLogout);

    pagesWidget = new QStackedWidget();
    pagesWidget->addWidget(createAddCoursePage());
    pagesWidget->addWidget(createManageUsersPage()); // [NEW] Page
    pagesWidget->addWidget(createLogsPage());
    pagesWidget->addWidget(createAttendancePage());
    pagesWidget->addWidget(createGradesPage());

    mainLayout->addLayout(sidebarLayout, 1);
    mainLayout->addWidget(pagesWidget, 4);

    connect(btnAddCourse, &QPushButton::clicked, this, &AdminPortal::showAddCoursePage);
    connect(btnUsers, &QPushButton::clicked, this, &AdminPortal::showManageUsersPage);
    connect(btnLogs, &QPushButton::clicked, this, &AdminPortal::showLogsPage);
    connect(btnAttendance, &QPushButton::clicked, this, &AdminPortal::showAttendancePage);
    connect(btnGrades, &QPushButton::clicked, this, &AdminPortal::showGradesPage);
    connect(btnPass, &QPushButton::clicked, this, &AdminPortal::changePassword);
    connect(btnLogout, &QPushButton::clicked, this, &AdminPortal::logout);
}

// --- 1. MANAGE COURSES PAGE ---
QWidget* AdminPortal::createAddCoursePage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);

    courseCodeInput = new QLineEdit(); courseCodeInput->setPlaceholderText("Course Code");
    courseNameInput = new QLineEdit(); courseNameInput->setPlaceholderText("Course Name");
    QPushButton *saveBtn = new QPushButton("Add Course");
    saveBtn->setStyleSheet("background-color: #27ae60; color: white; padding: 10px; font-weight: bold;");
    connect(saveBtn, &QPushButton::clicked, this, &AdminPortal::saveCourse);

    QTableWidget *table = new QTableWidget();
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"Course Code", "Course Name"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    vector<vector<string>> data = fm.getCoursesData();
    table->setRowCount(data.size());
    for(int i=0; i<data.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(data[i][0])));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(data[i][1])));
    }

    layout->addWidget(new QLabel("ADD NEW COURSE"));
    layout->addWidget(courseCodeInput); layout->addWidget(courseNameInput); layout->addWidget(saveBtn);
    layout->addSpacing(20);
    layout->addWidget(new QLabel("EXISTING COURSES:"));
    layout->addWidget(table);
    return page;
}

// --- 2. MANAGE USERS PAGE (ADD/REMOVE) ---
QWidget* AdminPortal::createManageUsersPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);

    // Section A: Add User
    QGroupBox *grpAdd = new QGroupBox("Register New User");
    QVBoxLayout *layAdd = new QVBoxLayout(grpAdd);

    newUserId = new QLineEdit(); newUserId->setPlaceholderText("User ID");
    newUserName = new QLineEdit(); newUserName->setPlaceholderText("Full Name");
    newUserPass = new QLineEdit(); newUserPass->setPlaceholderText("Password");
    newUserRole = new QComboBox(); newUserRole->addItems({"Student", "Teacher", "Admin"});

    QPushButton *btnAdd = new QPushButton("Register User");
    btnAdd->setStyleSheet("background-color: #27ae60; color: white; padding: 8px;");
    connect(btnAdd, &QPushButton::clicked, this, &AdminPortal::addUser);

    layAdd->addWidget(newUserId); layAdd->addWidget(newUserName);
    layAdd->addWidget(newUserPass); layAdd->addWidget(newUserRole); layAdd->addWidget(btnAdd);

    // Section B: Remove User
    QGroupBox *grpRem = new QGroupBox("Delete User");
    QVBoxLayout *layRem = new QVBoxLayout(grpRem);

    removeUserId = new QLineEdit(); removeUserId->setPlaceholderText("Enter User ID to Delete");
    QPushButton *btnRem = new QPushButton("Permanently Delete User");
    btnRem->setStyleSheet("background-color: #c0392b; color: white; padding: 8px;");
    connect(btnRem, &QPushButton::clicked, this, &AdminPortal::removeUser);

    layRem->addWidget(removeUserId); layRem->addWidget(btnRem);

    layout->addWidget(grpAdd);
    layout->addSpacing(10);
    layout->addWidget(grpRem);
    layout->addStretch();
    return page;
}

// --- 3. LOGS PAGE ---
QWidget* AdminPortal::createLogsPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);

    logsTable = new QTableWidget();
    logsTable->setColumnCount(1);
    logsTable->setHorizontalHeaderLabels({"Activity Details"});
    logsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QPushButton *refreshBtn = new QPushButton("Refresh Logs");
    refreshBtn->setStyleSheet("background-color: #2980b9; color: white; padding: 10px;");
    connect(refreshBtn, &QPushButton::clicked, this, &AdminPortal::refreshLogs);

    layout->addWidget(new QLabel("SYSTEM LOGS"));
    layout->addWidget(logsTable);
    layout->addWidget(refreshBtn);
    return page;
}

// --- 4. ATTENDANCE PAGE ---
QWidget* AdminPortal::createAttendancePage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);

    attendanceTable = new QTableWidget();
    attendanceTable->setColumnCount(4);
    attendanceTable->setHorizontalHeaderLabels({"Course", "Student ID", "Status", "Date"});
    attendanceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QPushButton *refreshBtn = new QPushButton("Refresh Attendance");
    refreshBtn->setStyleSheet("background-color: #2980b9; color: white; padding: 10px;");
    connect(refreshBtn, &QPushButton::clicked, this, &AdminPortal::refreshAttendance);

    layout->addWidget(new QLabel("ALL ATTENDANCE RECORDS"));
    layout->addWidget(attendanceTable);
    layout->addWidget(refreshBtn);
    return page;
}

// --- 5. GRADES PAGE ---
QWidget* AdminPortal::createGradesPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);

    gradesTable = new QTableWidget();
    gradesTable->setColumnCount(3);
    gradesTable->setHorizontalHeaderLabels({"Course", "Student ID", "Marks"});
    gradesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QPushButton *refreshBtn = new QPushButton("Refresh Grades");
    refreshBtn->setStyleSheet("background-color: #2980b9; color: white; padding: 10px;");
    connect(refreshBtn, &QPushButton::clicked, this, &AdminPortal::refreshGrades);

    layout->addWidget(new QLabel("ALL GRADE RECORDS"));
    layout->addWidget(gradesTable);
    layout->addWidget(refreshBtn);
    return page;
}

// --- PAGE NAVIGATION ---
void AdminPortal::showAddCoursePage() { pagesWidget->setCurrentIndex(0); }
void AdminPortal::showManageUsersPage() { pagesWidget->setCurrentIndex(1); }
void AdminPortal::showLogsPage() { pagesWidget->setCurrentIndex(2); refreshLogs(); }
void AdminPortal::showAttendancePage() { pagesWidget->setCurrentIndex(3); refreshAttendance(); }
void AdminPortal::showGradesPage() { pagesWidget->setCurrentIndex(4); refreshGrades(); }

// --- SLOTS (LOGIC) ---

void AdminPortal::saveCourse() {
    string code = courseCodeInput->text().toStdString();
    string name = courseNameInput->text().toStdString();
    if(code.empty() || name.empty()) return;

    fm.addCourse(code, name);
    QMessageBox::information(this, "Success", "Course Added!");
    courseCodeInput->clear(); courseNameInput->clear();
    // Note: Table auto-refresh nahi hoti yahan, app restart par update hogi
    // ya phir aap getCoursesData() dobara call kar ke table clear/fill kar sakte hain.
}

void AdminPortal::addUser() {
    if(newUserId->text().isEmpty() || newUserName->text().isEmpty() || newUserPass->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields!");
        return;
    }
    string res = fm.registerUser(
        newUserId->text().toStdString(),
        newUserName->text().toStdString(),
        newUserPass->text().toStdString(),
        newUserRole->currentText().toStdString()
        );

    if(res == "Success") {
        QMessageBox::information(this, "Success", "User Registered Successfully!");
        newUserId->clear(); newUserName->clear(); newUserPass->clear();
    } else {
        QMessageBox::warning(this, "Error", QString::fromStdString(res));
    }
}

void AdminPortal::removeUser() {
    string id = removeUserId->text().toStdString();
    if(id.empty()) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm", "Are you sure you want to delete User ID: " + removeUserId->text() + "?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // [NOTE] FileManager mein removeUser logic hona chahiye.
        // Agar aapke pass removeUser nahi hai, to ye function error dega.
        // Maine pichli bar FileManager mein removeUser add karwaya tha.
        string res = fm.removeUser(id);
        QMessageBox::information(this, "Status", QString::fromStdString(res));
        removeUserId->clear();
    }
}

void AdminPortal::refreshLogs() {
    vector<string> logs = fm.getLogsData();
    logsTable->setRowCount(logs.size());
    for(int i=0; i<logs.size(); i++) {
        logsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(logs[i])));
    }
}

void AdminPortal::refreshAttendance() {
    vector<vector<string>> data = fm.getAllAttendanceData();
    attendanceTable->setRowCount(data.size());
    for(int i=0; i<data.size(); i++) {
        attendanceTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(data[i][0])));
        attendanceTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(data[i][1])));
        attendanceTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(data[i][2])));
        attendanceTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(data[i][3])));
    }
}

void AdminPortal::refreshGrades() {
    vector<vector<string>> data = fm.getAllGradesData();
    gradesTable->setRowCount(data.size());
    for(int i=0; i<data.size(); i++) {
        gradesTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(data[i][0])));
        gradesTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(data[i][1])));
        gradesTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(data[i][2])));
    }
}

void AdminPortal::changePassword() {
    bool ok;
    QString newPass = QInputDialog::getText(this, "Security", "Enter New Password:", QLineEdit::Password, "", &ok);
    if (ok && !newPass.isEmpty()) {
        QMessageBox::information(this, "Status", QString::fromStdString(fm.changePassword(myId, newPass.toStdString())));
    }
}

void AdminPortal::logout() {
    this->close();
    MainWindow *login = new MainWindow();
    login->show();
}
