#pragma once

#include <QLayout>
#include <thread>
class QWidget;
class QTimer;

namespace Z {

// 使用类模拟枚举
struct Style final {
    static Style Row;
    static Style Col;
    static Style Square;
    int          state_ = 0;
    explicit Style(int state) : state_(state) { }
    // QString --> Style
    Style(const QString& str) {
        if(str == "Row") state_ = 0;
        else if(str == "Col")
            state_ = 1;
        else if(str == "Square")
            state_ = 2;
    }
    // Style --> QString
    operator const QString() {
        switch(state_) {
            case 0: return QStringLiteral("Row");
            case 1: return QStringLiteral("Col");
            case 2: return QStringLiteral("Square");
        }
        return QStringLiteral("");
    }

    bool operator==(Style const& b) {
        return this->state_ == b.state_;
    }

    bool operator==(QString const& b) {
        return this->state_ == (Style)b;
    }
};

class FlowLayout : public QLayout {
    Q_OBJECT
public:
    Q_PROPERTY(qreal refWidth_ READ refWidth WRITE setRefWidth);
    Q_PROPERTY(Style style_ READ style WRITE setStyle);
    FlowLayout(QWidget* parent = nullptr);
    ~FlowLayout();
    // QLayout interface
    int          count() const override;
    void         addItem(QLayoutItem* item) override;
    void         setGeometry(const QRect& r) override;
    QSize        sizeHint() const override;
    QLayoutItem* itemAt(int index) const override;
    QLayoutItem* takeAt(int index) override;

    void  setRefWidth(qreal width);
    qreal refWidth();
    void  setStyle(Style style);
    Style style();
    qreal innerHeight();

protected:
    // this means every items's width is equal
    void doColLayout();
    // this means every items's height is equal(not complete)
    void doRowLayout();
    // this means every items's width and height is equal
    void doSquareLayout();
    void setInnerHeight(qreal height);

private:
    Style               style_ = Style::Col;
    QList<QLayoutItem*> list_;
    qreal               refwidth_ = 300;
    qreal               innerHeight_;
};
}    // namespace Z
