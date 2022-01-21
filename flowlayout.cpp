#include "flowlayout.h"
#include <QDebug>
#include <QRect>
#include <QTimer>
#include <QWidget>

#include <functional>
#include <iostream>

namespace Z {
FlowLayout::FlowLayout(QWidget* parent) : QLayout(parent), innerHeight_(0), timer_ { new QTimer } {
    // delay refresh
    timer_->setInterval(300);
    connect(timer_, &QTimer::timeout, this, &FlowLayout::update);
}
FlowLayout::~FlowLayout() {
    delete timer_;
    QLayoutItem* item;
    while((item = FlowLayout::takeAt(0))) {
        delete item;
        item = nullptr;
    }
}

void FlowLayout::addItem(QLayoutItem* item) {
    list_.append(item);
    // if there has no update, then item will overlap before resize
    timer_->start();
}
int FlowLayout::count() const {
    return list_.length();
}
QLayoutItem* FlowLayout::itemAt(int index) const {
    return index >= 0 && index < list_.length() ? list_.at(index) : nullptr;
}
QSize FlowLayout::sizeHint() const {
    return QSize(geometry().width(), innerHeight_);
}

QLayoutItem* FlowLayout::takeAt(int index) {
    return index >= 0 && index < list_.length() ? list_.takeAt(index) : nullptr;
}
void FlowLayout::setGeometry(const QRect& r) {
    QLayout::setGeometry(r);
    doMonoLayout();
}
void FlowLayout::setWidgetWidth(size_t size) {
    this->setRefWidth(size);
}
void FlowLayout::setRefWidth(qreal width) {
    this->refwidth_ = width;
}
size_t FlowLayout::widgetWidth() {
    return refwidth_;
}

qreal FlowLayout::innerHeight() {
    return innerHeight_;
}

const QList<QLayoutItem*> FlowLayout::list() const {
    return list_;
}

void FlowLayout::doMonoLayout() {
    assert(refwidth_ != -1);
    if(list_.length() <= 0) return;
    // a line has n widgets an n-1 spacing
    // n*refwidth_ + (n-1)*spacing = this->width()
    // n*(refwidth_+spacing) - spacing = this.width()
    int n = (this->geometry().width() + spacing()) / (refwidth_ + spacing());
    // n should always less than list_.length()
    n = std::min(n, list_.length());

    qreal        realWidth = refwidth_ + (this->geometry().width() - n * refwidth_) / n;
    QVector<int> yFlags(n);

    qreal realHeight;
    int   min = 0;
    for(auto& item: list_) {
        // realHieght = realWidth*(y/x)
        // ry/rx=y/x ==> ry=rx*y/x
        realHeight = realWidth * item->sizeHint().rheight() / item->sizeHint().rwidth();
        item->setGeometry(QRect(min * (spacing() + realWidth), yFlags[min], realWidth, realHeight));
        yFlags[min] += realHeight + spacing();
        // choose the min elements's index
        for(int i = 0; i < yFlags.size(); ++i) {
            if(yFlags[i] < yFlags[min]) min = i;
        }
    }
    innerHeight_ = *std::max_element(yFlags.begin(), yFlags.end());
    emit this->innerHeightChanged(innerHeight_);
    // WARNING: this code should be delete then
    // set parent's size
    this->parentWidget()->resize(this->geometry().width(), innerHeight_);
}
}    // namespace Z
