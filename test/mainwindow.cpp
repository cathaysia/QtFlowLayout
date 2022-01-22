#include "mainwindow.hpp"

#include <QDebug>
#include <QHBoxLayout>
#include <QRandomGenerator>
#include <QTime>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), container(new QWidget(this)) {
    this->setCentralWidget(container);
    container->setLayout(new Z::FlowLayout);

    QPalette pa;
    for(int i = 0; i < 10; ++i) {
        pa.setColor(QPalette::Window, Qt::white);
        auto lbl = new QLabel();
        lbl->setStyleSheet("QLabel{background-color:rgb(200,101,102);}");
        lbl->resize(300, 300);
        qDebug() << lbl->sizeHint();
        container->layout()->addWidget(lbl);
    }
}
