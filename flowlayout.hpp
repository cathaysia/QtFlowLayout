#pragma once

#include <QLayout>
#include <thread>
class QWidget;
class QTimer;

namespace Z {

class FlowLayout : public QLayout {
    Q_OBJECT
public:
    enum class Style : unsigned char { Cow, Square };
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

    [[deprecated]] const QList<QLayoutItem*> list() const;
    [[deprecated]] void                      setWidgetWidth(size_t size);
    [[deprecated]] qreal                     innerHeight();
    [[deprecated]] inline size_t             widgetWidth();

    void  setRefWidth(qreal width);
    qreal refWidth();
    void  setStyle(Style style);
    Style style();

signals:
    void innerHeightChanged(qreal innerHeight);
    void refWidthChanged(qreal refwidth);
    void styleChanged(Style style);

protected:
    // this means every items's width is equal
    void doMonoLayout();
    // void doRowLayout();
    // this means every items's width and height is equal
    void doSquareLayout();

private:
    Style               style_ = Style::Cow;
    QList<QLayoutItem*> list_;
    qreal               refwidth_ = -1;
    qreal               innerHeight_;
    QTimer*             timer_;
};

}    // namespace Z
