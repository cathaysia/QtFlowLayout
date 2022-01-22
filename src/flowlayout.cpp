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
        this->setGeometry(this->geometry());
    });
    // WARNING: this code should be delete then
    connect(this, &FlowLayout::innerHeightChanged, [this](qreal innerHeight) {
        qDebug() << "innerHeightChanged 信号被触发";
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
    if(style_ == Style::Cow) {
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
    if(refwidth_ <= 0) refwidth_ = 300;
    if(list_.length() <= 0) return;
    int n = (this->geometry().width() + spacing()) / (refwidth_ + spacing());
    // n should always less than list_.length()
    n = std::min(n, list_.length());
    n = n == 0 ? 1 : n;

    qreal realWidth = refwidth_ + (this->geometry().width() - n * refwidth_) / n;

    /**
     * For this layout, can be abstract to a algorithm:
     * Give a array A, then split it to some sub-array,
     * ask every sub-array's sum asme as number b,
     * and the sum of every sub-array's variance be min
     *
     * I just take a simple algorithm:
     * 1. Count how many sub-array need, and mark as n
     * 2. order array A, then extract the smallest n items
     * 3. shuffle array A
     * 4. split A to n sub-array, and every array's sum less than b
     * 5. Choose items that get from step 2, then append sub-array
     */
    QList<QLayoutItem*> cpy = list_;
    std::make_heap(cpy.begin(), cpy.begin() + n, [](QLayoutItem* a, QLayoutItem* b) {
        return a->sizeHint().rwidth() > a->sizeHint().rwidth();
    });
    for(int i = n; i < cpy.length(); ++i) {
        if(cpy[i] < cpy[0]) {
            std::swap(cpy[i], cpy[0]);
            std::make_heap(cpy.begin(), cpy.begin() + n, [](QLayoutItem* a, QLayoutItem* b) {
                return a->sizeHint().rwidth() > b->sizeHint().rwidth();
            });
        }
    }
    // From now, list[0]->list[n] is the smallest items.
    // then come to step 4
    QList<QList<QLayoutItem*>> container;

    auto sumItems = [](QList<QLayoutItem*>& item) -> qreal {
        qreal sum = 0;
        for(auto& it: item) { sum += it->sizeHint().rwidth(); }
        return sum;
    };
    QList<QLayoutItem*> temp;
    for(int i = n; i < cpy.length(); ++i) {
        if(sumItems(temp) + cpy[i]->sizeHint().rwidth() > geometry().size().width()) {
            container.push_back(temp);
            temp.clear();
        }
        temp.push_back(cpy[n]);
    }
    container.push_back(temp);
    // for step 5
    // sort from largest to smallest
    std::sort(container.begin(), container.end(), [&](auto& a, auto& b) {
        return sumItems(a) > sumItems(b);
    });
    // sort from smallest to largest
    std::sort(cpy.begin(), cpy.end(), [](QLayoutItem* a, QLayoutItem* b) {
        return a->sizeHint().rwidth() < a->sizeHint().rwidth();
    });
    for(int i = 0; i < n; ++i) {
        //
        if(i == container.length() || (i == cpy.size())) break;
        container[i].push_back(cpy[i]);
    }
    // now, set Geometry
    qreal posX;
    qreal posY = 0;
    for(QList<QLayoutItem*>& lst: container) {
        posX = 0;
        for(QLayoutItem* item: lst) {
            item->setGeometry(QRect(posX, posY, 110, 200));
            posX = 120;
        }
        posY += 210;
        // // sumWidth/width = realHeigth/refwidth_
        // // realHeigth = sumWidth * refwidth_/realWidth
        // qreal realHeigth = sumItems(lst) * refwidth_ / geometry().width();
        // posX             = 0;
        // qDebug() << "lst.size: " << lst.size();
        // for(QLayoutItem* item: lst) {
        //     // itemHeight/ realHeigth = itemWidth/realWidth
        //     // realWidth = itemWidth * realHeigth / itemHeight
        //     realWidth = item->sizeHint().rwidth() * realHeigth / item->sizeHint().rheight();
        //     item->setGeometry(QRect(posX, posY, realWidth, realHeigth));
        //     qDebug() << item->geometry();
        //     posX += spacing() + realWidth;
        // }
        // posY += realHeigth + spacing();
    }
    this->setInnerHeight(posY);
}
}    // namespace Z
