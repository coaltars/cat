#pragma once
#include <QTabBar>

class TabBar : public QTabBar {
    Q_OBJECT
public:
    explicit TabBar(QWidget* parent = nullptr);
    
signals:
    void tabCloseRequested(int index);
    
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    QSize tabSizeHint(int index) const override;
};