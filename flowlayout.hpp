#pragma once

#include <QLayout>
#include <thread>
class QWidget;
class QTimer;

namespace Z {

class FlowLayout : public QLayout {
    Q_OBJECT
public:
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

    void          setRefWidth(qreal width);
    inline size_t widgetWidth();

signals:
    void innerHeightChanged(qreal innerHeight);

protected:
    // this means every items's width is equal
    void doMonoLayout();
    // void doRowLayout();
    // void doSquareLayout();

private:
    QList<QLayoutItem*> list_;
    qreal               refwidth_ = -1;
    qreal               innerHeight_;
    QTimer*             timer_;
};

}    // namespace Z
