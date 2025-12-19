#include "teacherportal.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>

// [FIX] Constructor ab ID bhi accept kar raha hai
TeacherPortal::TeacherPortal(string id, QString name, QWidget *parent)
    : QMainWindow(parent), myId(id), teacherName(name)
{
    setWindowTitle("Teacher Portal - " + teacherName);
    resize(1000, 700);
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    QVBoxLayout *sidebarLayout = new QVBoxLayout();

    QLabel *welcome = new QLabel("Welcome,\n" + teacherName);
    welcome->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 20px;");

    QPushButton *btnAtt = new QPushButton("Attendance");
    QPushButton *btnGrades = new QPushButton("Grades");
    QPushButton *btnAssign = new QPushButton("Assignments");
    QPushButton *btnPass = new QPushButton("Change Password");
    QPushButton *btnLogout = new QPushButton("Logout");

    QString btnStyle = "QPushButton { text-align: left; padding: 15px; background-color: #34495e; color: white; border-radius: 5px; } QPushButton:hover { background-color: #2c3e50; }";
    btnAtt->setStyleSheet(btnStyle);
    btnGrades->setStyleSheet(btnStyle);
    btnAssign->setStyleSheet(btnStyle);
    btnPass->setStyleSheet(btnStyle);
    btnLogout->setStyleSheet("background-color: #e74c3c; color: white; padding: 15px; border-radius: 5px;");

    sidebarLayout->addWidget(welcome);
    sidebarLayout->addWidget(btnAtt);
    sidebarLayout->addWidget(btnGrades);
    sidebarLayout->addWidget(btnAssign);
    sidebarLayout->addWidget(btnPass);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(btnLogout);

    pagesWidget = new QStackedWidget();
    pagesWidget->addWidget(createAttendancePage());
    pagesWidget->addWidget(createGradesPage());
    pagesWidget->addWidget(createAssignmentPage());

    mainLayout->addLayout(sidebarLayout, 1);
    mainLayout->addWidget(pagesWidget, 4);

    connect(btnAtt, &QPushButton::clicked, this, &TeacherPortal::showAttendancePage);
    connect(btnGrades, &QPushButton::clicked, this, &TeacherPortal::showGradesPage);
    connect(btnAssign, &QPushButton::clicked, this, &TeacherPortal::showAssignmentPage);
    connect(btnPass, &QPushButton::clicked, this, &TeacherPortal::changePassword);
    connect(btnLogout, &QPushButton::clicked, this, &TeacherPortal::logout);
}

// --- ATTENDANCE PAGE ---
QWidget* TeacherPortal::createAttendancePage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);

    attCourseInput = new QLineEdit(); attCourseInput->setPlaceholderText("Course Code");
    attStudentIdInput = new QLineEdit(); attStudentIdInput->setPlaceholderText("Student ID");
    attStatusBox = new QComboBox(); attStatusBox->addItems({"Present", "Absent"});
    QPushButton *save = new QPushButton("Mark Attendance");
    save->setStyleSheet("background-color: #27ae60; color: white; padding: 10px;");
    connect(save, &QPushButton::clicked, this, &TeacherPortal::saveAttendance);

    QTableWidget *table = new QTableWidget();
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Course", "Student ID", "Status", "Date"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    vector<vector<string>> data = fm.getAllAttendanceData();
    table->setRowCount(data.size());
    for(int i=0; i<data.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(data[i][0])));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(data[i][1])));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(data[i][2])));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(data[i][3])));
    }

    layout->addWidget(new QLabel("MARK ATTENDANCE"));
    layout->addWidget(attCourseInput); layout->addWidget(attStudentIdInput); layout->addWidget(attStatusBox); layout->addWidget(save);
    layout->addSpacing(20);
    layout->addWidget(new QLabel("RECENT ATTENDANCE RECORDS:"));
    layout->addWidget(table);
    return page;
}

// --- GRADES PAGE ---
QWidget* TeacherPortal::createGradesPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);

    gradeCourseInput = new QLineEdit(); gradeCourseInput->setPlaceholderText("Course Code");
    gradeStudentIdInput = new QLineEdit(); gradeStudentIdInput->setPlaceholderText("Student ID");
    gradeMarksInput = new QLineEdit(); gradeMarksInput->setPlaceholderText("Marks");
    QPushButton *save = new QPushButton("Upload Grade");
    save->setStyleSheet("background-color: #2980b9; color: white; padding: 10px;");
    connect(save, &QPushButton::clicked, this, &TeacherPortal::saveGrade);

    QTableWidget *table = new QTableWidget();
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Course", "Student ID", "Marks"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    vector<vector<string>> data = fm.getAllGradesData();
    table->setRowCount(data.size());
    for(int i=0; i<data.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(data[i][0])));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(data[i][1])));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(data[i][2])));
    }

    layout->addWidget(new QLabel("UPLOAD GRADES"));
    layout->addWidget(gradeCourseInput); layout->addWidget(gradeStudentIdInput); layout->addWidget(gradeMarksInput); layout->addWidget(save);
    layout->addSpacing(20);
    layout->addWidget(new QLabel("UPLOADED GRADES:"));
    layout->addWidget(table);
    return page;
}

// --- ASSIGNMENT PAGE (WITH DEADLINE) ---
QWidget* TeacherPortal::createAssignmentPage() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);

    assignCourseInput = new QLineEdit(); assignCourseInput->setPlaceholderText("Course Code");
    assignTitleInput = new QLineEdit(); assignTitleInput->setPlaceholderText("Assignment Title");

    // [CHANGE] "Submission Deadline" wala text hata diya
    assignDeadlineInput = new QDateEdit();
    assignDeadlineInput->setDate(QDate::currentDate().addDays(7)); // Default 1 week
    assignDeadlineInput->setCalendarPopup(true);
    assignDeadlineInput->setToolTip("Select Deadline Date"); // Mouse rakhne par hint milegi

    QPushButton *save = new QPushButton("Post Assignment");
    save->setStyleSheet("background-color: #8e44ad; color: white; padding: 10px;");
    connect(save, &QPushButton::clicked, this, &TeacherPortal::saveAssignment);

    QTableWidget *table = new QTableWidget();
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Course", "Title", "Deadline"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    vector<vector<string>> data = fm.getAssignmentsData();
    table->setRowCount(data.size());
    for(int i=0; i<data.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(data[i][0])));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(data[i][1])));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(data[i][2])));
    }

    layout->addWidget(new QLabel("POST ASSIGNMENT"));
    layout->addWidget(assignCourseInput);
    layout->addWidget(assignTitleInput);
    layout->addWidget(assignDeadlineInput); // Text hat gaya, sirf Date Picker hai ab
    layout->addWidget(save);
    layout->addSpacing(20);
    layout->addWidget(new QLabel("POSTED ASSIGNMENTS:"));
    layout->addWidget(table);
    return page;
}

void TeacherPortal::showAttendancePage() { pagesWidget->setCurrentIndex(0); }
void TeacherPortal::showGradesPage() { pagesWidget->setCurrentIndex(1); }
void TeacherPortal::showAssignmentPage() { pagesWidget->setCurrentIndex(2); }

void TeacherPortal::saveAttendance() {
    fm.markAttendance(attCourseInput->text().toStdString(), attStudentIdInput->text().toStdString(), attStatusBox->currentText().toStdString());
    QMessageBox::information(this, "Success", "Attendance Marked! (Restart/Reopen to see)");
}
void TeacherPortal::saveGrade() {
    fm.addGrade(gradeCourseInput->text().toStdString(), gradeStudentIdInput->text().toStdString(), gradeMarksInput->text().toDouble());
    QMessageBox::information(this, "Success", "Grade Saved!");
}
void TeacherPortal::saveAssignment() {
    string date = assignDeadlineInput->date().toString("yyyy-MM-dd").toStdString();
    fm.addAssignment(assignCourseInput->text().toStdString(), assignTitleInput->text().toStdString(), date);
    QMessageBox::information(this, "Success", "Assignment Posted with Deadline!");
}
void TeacherPortal::changePassword() {
    bool ok;
    QString newPass = QInputDialog::getText(this, "Security", "Enter New Password:", QLineEdit::Password, "", &ok);
    if (ok && !newPass.isEmpty()) {
        QMessageBox::information(this, "Status", QString::fromStdString(fm.changePassword(myId, newPass.toStdString())));
    }
}
void TeacherPortal::logout() {
    this->close();
    MainWindow *login = new MainWindow();
    login->show();
}
