#pragma once

#include <QLayout>
#include <thread>
class QWidget;
class QTimer;

namespace Z {

class FlowLayout : public QLayout {
    Q_OBJECT
public:
    enum class Style : unsigned char { Row, Col, Square };
    Q_ENUM(Style);
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

signals:
    void innerHeightChanged(qreal innerHeight);
    void refWidthChanged(qreal refwidth);
    void styleChanged(Style style);

protected:
    // this means every items's width is equal
    void doColLayout();
    // this means every items's height is equal(not complete)
    void doRowLayout();
    // this means every items's width and height is equal
    // WARNING: This function can not work normally at now
    void doSquareLayout();
    void setInnerHeight(qreal height);

private:
    Style               style_ = Style::Col;
    QList<QLayoutItem*> list_;
    qreal               refwidth_ = 300;
    qreal               innerHeight_;
};

}    // namespace Z
