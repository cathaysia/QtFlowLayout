#include "flowlayout.hpp"
#include <QDebug>
#include <QRect>
#include <QTimer>
#include <QWidget>

#include <algorithm>
#include <functional>
#include <iostream>
#include <typeinfo>

namespace Z {
FlowLayout::FlowLayout(QWidget* parent) : QLayout(parent), innerHeight_(0) {
    // delay refresh
    connect(this, &FlowLayout::styleChanged, [this]() {
        this->update();
    });
    connect(this, &FlowLayout::refWidthChanged, [this]() {
        this->update();
    });
    // WARNING: this code should be delete then
    connect(this, &FlowLayout::innerHeightChanged, [this](qreal innerHeight) {
        this->parentWidget()->resize(this->geometry().width(), innerHeight);
    });
}
FlowLayout::~FlowLayout() {
    QLayoutItem* item;
    while((item = FlowLayout::takeAt(0))) {
        delete item;
        item = nullptr;
    }
}

void FlowLayout::addItem(QLayoutItem* item) {
    list_.append(item);
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
    if(style_ == Style::Col) {
        doMonoLayout();
    } else if(style_ == Style::Square) {
        doSquareLayout();
    } else if(style_ == Style::Row) {
        doRowLayout();
    }
}
void FlowLayout::setWidgetWidth(size_t size) {
    this->setRefWidth(size);
}
void FlowLayout::setRefWidth(qreal width) {
    this->refwidth_ = width;
    emit this->refWidthChanged(width);
}
qreal FlowLayout::refWidth() {
    return refwidth_;
}
size_t FlowLayout::widgetWidth() {
    return refWidth();
}
void FlowLayout::setStyle(Style style) {
    this->style_ = style;
    emit this->styleChanged(style_);
}

FlowLayout::Style FlowLayout::style() {
    return this->style_;
}

qreal FlowLayout::innerHeight() {
    return innerHeight_;
}

const QList<QLayoutItem*> FlowLayout::list() const {
    return list_;
}
void FlowLayout::setInnerHeight(qreal height) {
    if(height == this->innerHeight_) return;
    this->innerHeight_ = height;
    emit this->innerHeightChanged(this->innerHeight_);
}

void FlowLayout::doMonoLayout() {
    if(refwidth_ <= 0) refwidth_ = 300;
    if(list_.length() <= 0) return;
    // a line has n widgets an n-1 spacing
    // n*refwidth_ + (n-1)*spacing = this->width()
    // n*(refwidth_+spacing) - spacing = this.width()
    int n = (this->geometry().width() + spacing()) / (refwidth_ + spacing());
    // n should always less than list_.length()
    n = std::min(n, list_.length());
    n = n == 0 ? 1 : n;

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
    this->setInnerHeight(*std::max_element(yFlags.begin(), yFlags.end()));
}

void FlowLayout::doSquareLayout() {
    if(refwidth_ <= 0) refwidth_ = 300;
    if(list_.length() <= 0) return;
    int n = (this->geometry().width() + spacing()) / (refwidth_ + spacing());
    // n should always less than list_.length()
    n = std::min(n, list_.length());
    n = n == 0 ? 1 : n;

    qreal realWidth = refwidth_ + (this->geometry().width() - n * refwidth_) / n;
    int   posY      = 0;
    int   posX      = 0;
    for(auto& item: list_) {
        if(posX > geometry().width()) {
            posY += realWidth + spacing();
            posX = 0;
        };
        item->setGeometry(QRect(posX, posY, realWidth, realWidth));
        posX += spacing() + realWidth;
    }
    this->setInnerHeight(posY + spacing() + realWidth);
}
void FlowLayout::doRowLayout() {
    // Just a simple algorithm, scale the last item evert line
    // here refwidth_ become realHeight
    if(refwidth_ <= 0) refwidth_ = 300;
    if(list_.length() <= 0) return;

    // picW/picH = realW / realH
    auto realW = [this](QLayoutItem* item) -> qreal {
        return item->sizeHint().rwidth() * refwidth_ / item->sizeHint().rheight();
    };

    int posX = 0;
    int posY = 0;
    for(QLayoutItem* it: list_) {
        if(posX + realW(it) > geometry().width()) {
            it->setGeometry(QRect(posX, posY, geometry().width() - spacing() - posX, refwidth_));
            posX = 0;
            posY += spacing() + refwidth_;
            continue;
        }
        it->setGeometry(QRect(posX, posY, realW(it), refwidth_));
        posX += spacing() + realW(it);
    }
    this->setInnerHeight(posY + refwidth_);
}
}    // namespace Z
