#pragma once

#include <QLabel>
#include <QMainWindow>
#include <QResizeEvent>
#include <QWidget>

#include <flowlayout.hpp>

class MainWindow : public QMainWindow {
    Q_OBJECT;

public:
    explicit MainWindow(QWidget* parent = nullptr);
    void resizeEvent(QResizeEvent* event) override;

private:
    QWidget* container_;
};
