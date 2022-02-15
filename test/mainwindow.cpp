#include "mainwindow.hpp"

#include <QDebug>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QRandomGenerator>
#include <QScrollArea>
#include <QTime>
#include <QToolBar>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), container_(new QWidget(this)) {
    QScrollArea* area = new QScrollArea(this);
    area->setWidgetResizable(false);
    this->setCentralWidget(area);
    area->setWidget(container_);

    auto lay = new Z::FlowLayout;
    container_->setLayout(lay);

    auto menbar = menuBar()->addMenu("Menu");

    menbar->addAction("RefWidth", [lay]() {
        lay->setRefWidth(700);
    });

    auto act       = menuBar()->addAction("Change Style");
    auto styleMenu = new QMenu;
    act->setMenu(styleMenu);
    styleMenu->addAction("Row", [lay]() {
        lay->setStyle(Z::FlowLayout::Style::Row);
    });

    styleMenu->addAction("Col", [lay]() {
        lay->setStyle(Z::FlowLayout::Style::Col);
    });
    styleMenu->addAction("Square", [lay]() {
        lay->setStyle(Z::FlowLayout::Style::Square);
    });
    auto rgb = []() -> unsigned {
        return QRandomGenerator::system()->bounded(0, 255);
    };
    for(int i = 0; i < 10; ++i) {
        auto lbl = new QLabel();
        lbl->setStyleSheet(QString("QLabel{background-color:rgb(%1,%2,%3);}").arg(rgb()).arg(rgb()).arg(rgb()));
        qDebug() << lbl->sizeHint();
        container_->layout()->addWidget(lbl);
    }
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    container_->resize(this->size().width(), container_->size().height());
}
