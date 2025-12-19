#include "mainwindow.h"
#include "studentportal.h"
#include "teacherportal.h"
#include "adminportal.h"
#include <QFile>        // File check karne ke liye
#include <QMessageBox>  // Error dikhane ke liye

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("University LMS");
    resize(800, 600);

    centralWidget = new QWidget(this);

    // --- PHOTO PATH ---
    // Agar aapke PC par path different hai to yahan change karein
    // Make sure slashes Forward (/) hon.
    QString imagePath = "C:/Users/DELL/Desktop/final_bg.png";

    // --- PHOTO CHECKER ---
    if (!QFile::exists(imagePath)) {
        QMessageBox::critical(this, "Image Error",
                              "Photo nahi mili!\n\nPath: " + imagePath + "\n\nCheck karein ke naam 'final_bg.png' hi hai na?");
    }

    // [IMPORTANT FIX IS HERE]
    // 1. Widget ko aik khaas naam diya "mainBgWidget"
    centralWidget->setObjectName("mainBgWidget");

    // 2. Stylesheet mein "#mainBgWidget" use kiya.
    // Iska matlab hai: "Ye photo sirf us widget par lagao jiska naam mainBgWidget hai".
    // Is se photo buttons ke andar nahi jayegi.
    centralWidget->setStyleSheet("QWidget#mainBgWidget { border-image: url(" + imagePath + ") 0 0 0 0 stretch stretch; }");


    setCentralWidget(centralWidget);

    QGridLayout *outerLayout = new QGridLayout(centralWidget);

    QFrame *loginCard = new QFrame(this);
    loginCard->setFixedWidth(400);

    // Login box transparent (0.95 opacity)
    // "border-image: none;" yahan bhi zaroori hai taake card saaf rahe.
    loginCard->setStyleSheet("QFrame { border-image: none; background-color: rgba(255, 255, 255, 0.95); border-radius: 10px; border: 1px solid #bdc3c7; }");

    QVBoxLayout *cardLayout = new QVBoxLayout(loginCard);
    cardLayout->setSpacing(15);
    cardLayout->setContentsMargins(30, 30, 30, 30);

    QLabel *title = new QLabel("LMS LOGIN");
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #2c3e50; border: none; background: transparent;");
    title->setAlignment(Qt::AlignCenter);

    QLabel *roleLabel = new QLabel("Select Your Role:");
    roleLabel->setStyleSheet("font-weight: bold; border: none; background: transparent;");

    roleSelect = new QComboBox(this);
    roleSelect->addItems({"Student", "Teacher", "Admin"});
    // Inputs ko bhi border-image: none de diya taake safe side rahein
    roleSelect->setStyleSheet("QComboBox { border-image: none; padding: 8px; border: 1px solid #ccc; border-radius: 5px; background-color: #fafafa; }");

    idInput = new QLineEdit(this);
    idInput->setPlaceholderText("User ID");
    idInput->setStyleSheet("QLineEdit { border-image: none; padding: 10px; border: 1px solid #ccc; border-radius: 5px; background-color: white; }");

    passInput = new QLineEdit(this);
    passInput->setPlaceholderText("Password");
    passInput->setEchoMode(QLineEdit::Password);
    passInput->setStyleSheet("QLineEdit { border-image: none; padding: 10px; border: 1px solid #ccc; border-radius: 5px; background-color: white; }");

    btnLogin = new QPushButton("Login", this);
    btnLogin->setCursor(Qt::PointingHandCursor);
    btnLogin->setStyleSheet("QPushButton { border-image: none; background-color: #2980b9; color: white; padding: 12px; border-radius: 5px; font-weight: bold; border: none; } QPushButton:hover { background-color: #3498db; }");

    btnRegister = new QPushButton("Create Account", this);
    btnRegister->setCursor(Qt::PointingHandCursor);
    btnRegister->setStyleSheet("QPushButton { border-image: none; background-color: #27ae60; color: white; padding: 12px; border-radius: 5px; font-weight: bold; border: none; } QPushButton:hover { background-color: #2ecc71; }");

    statusLabel = new QLabel("", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: red; border: none; background: transparent;");

    cardLayout->addWidget(title);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(roleLabel);
    cardLayout->addWidget(roleSelect);
    cardLayout->addWidget(idInput);
    cardLayout->addWidget(passInput);
    cardLayout->addWidget(btnLogin);
    cardLayout->addWidget(btnRegister);
    cardLayout->addWidget(statusLabel);

    outerLayout->addWidget(loginCard, 0, 0, Qt::AlignCenter);

    connect(btnLogin, &QPushButton::clicked, this, &MainWindow::loginClicked);
    connect(btnRegister, &QPushButton::clicked, this, &MainWindow::registerClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::loginClicked() {
    string id = idInput->text().toStdString();
    string pass = passInput->text().toStdString();
    string selectedRole = roleSelect->currentText().toStdString();

    if(id.empty() || pass.empty()) {
        statusLabel->setText("Please fill all fields");
        return;
    }

    string result = fm.checkLogin(id, pass, selectedRole);

    if(result == "FAIL") {
        statusLabel->setText("Invalid Credentials or Wrong Role!");
    }
    else {
        int comma = result.find(',');
        string role = result.substr(0, comma);
        string name = result.substr(comma + 1);

        this->hide();

        if(role == "Student") {
            StudentPortal *portal = new StudentPortal(id, name);
            portal->showMaximized();
        }
        else if (role == "Teacher") {
            TeacherPortal *tPortal = new TeacherPortal(id, QString::fromStdString(name));
            tPortal->showMaximized();
        }
        else if (role == "Admin") {
            AdminPortal *aPortal = new AdminPortal(id, QString::fromStdString(name));
            aPortal->showMaximized();
        }
    }
}

void MainWindow::registerClicked() {
    QDialog dlg(this);
    dlg.setWindowTitle("Register");
    QFormLayout form(&dlg);

    QLineEdit *rId = new QLineEdit(&dlg);
    QLineEdit *rName = new QLineEdit(&dlg);
    QLineEdit *rPass = new QLineEdit(&dlg);
    QComboBox *rRole = new QComboBox(&dlg);
    rRole->addItems({"Student", "Teacher", "Admin"});

    QPushButton *save = new QPushButton("Register", &dlg);

    form.addRow("ID:", rId);
    form.addRow("Name:", rName);
    form.addRow("Password:", rPass);
    form.addRow("Role:", rRole);
    form.addRow(save);

    connect(save, &QPushButton::clicked, [&](){
        string res = fm.registerUser(
            rId->text().toStdString(),
            rName->text().toStdString(),
            rPass->text().toStdString(),
            rRole->currentText().toStdString()
            );

        if(res == "Success") {
            QMessageBox::information(this, "Success", "Registered!");
            dlg.accept();
        } else {
            QMessageBox::warning(this, "Error", QString::fromStdString(res));
        }
    });
    dlg.exec();
}
