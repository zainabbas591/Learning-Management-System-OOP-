#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QDialog>
#include <QComboBox> //
#include <QFormLayout>
#include <QMessageBox>
#include <QGridLayout>
#include <QFrame>
#include "FileManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loginClicked();
    void registerClicked();

private:
    QWidget *centralWidget;

    // UI Elements
    QComboBox *roleSelect; // [NEW] Dropdown for Role
    QLineEdit *idInput;
    QLineEdit *passInput;

    QPushButton *btnLogin;
    QPushButton *btnRegister;
    QLabel *statusLabel;

    FileManager fm;
};

#endif // MAINWINDOW_H
