#pragma once

#include <QLabel>
#include <QMainWindow>
#include <QWidget>

#include <flowlayout.hpp>
class MainWindow : public QMainWindow {
    Q_OBJECT;

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    QWidget* container;
};
