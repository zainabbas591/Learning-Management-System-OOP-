#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QFile> // [FIX] Ye missing tha, iske bina error aayega
#include "FileManager.h"

// Wo 'totalFilesOpened' wali line yahan se delete kar di gayi hai (Sahi hai)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Auto-create folders
    if (!QDir("data").exists()) QDir().mkdir("data");

    // Auto-create dummy course if not exists
    FileManager fm;
    if (!QFile("data/courses.txt").exists()) {
        fm.addCourse("CS101", "Introduction to Computing");
    }

    MainWindow w;
    w.show();

    return a.exec();
}
