#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QWidget>

#include "mainwindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow   w;
    w.show();
    return app.exec();
}
