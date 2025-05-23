#include "editor/tab.h"
#include "core/theme.h"
#include <QMouseEvent>
#include <QPainter>
#include <QFontMetrics>

TabBar::TabBar(QWidget* parent) : QTabBar(parent) {
    setExpanding(false);
    setMovable(true);
    setTabsClosable(false);
    
    auto& theme = Theme::instance();
    setFont(theme.font);
    
    QString style = QString(
        "QTabBar::tab {"
        "background: %1;"
        "color: %2;"
        "border: 1px solid %3;"
        "border-bottom: none;"
        "padding: 4px 20px 4px 12px;" 
        "margin-right: 1px;"
        "}"
        "QTabBar::tab:selected {"
        "background: %4;"
        "color: %5;"
        "}"
        "QTabBar::tab:hover {"
        "background: %3;"
        "}"
    ).arg(theme.tab_bg.name())
     .arg(theme.fg.name())
     .arg(theme.border.name())
     .arg(theme.bg.name())
     .arg(theme.accent.name());
    
    setStyleSheet(style);
}

void TabBar::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton) {
        int index = tabAt(event->pos());
        if (index >= 0) {
            emit tabCloseRequested(index);
            return;
        }
    }
    
    if (event->button() == Qt::LeftButton) {
        int index = tabAt(event->pos());
        if (index >= 0) {
            QRect tabRect = this->tabRect(index);
            QRect closeRect(tabRect.right() - 16, tabRect.top() + 4, 12, 12);
            if (closeRect.contains(event->pos())) {
                emit tabCloseRequested(index);
                return;
            }
        }
    }
    
    QTabBar::mousePressEvent(event);
}

void TabBar::paintEvent(QPaintEvent* event) {
    QTabBar::paintEvent(event);
    
    QPainter painter(this);
    auto& theme = Theme::instance();
    
    for (int i = 0; i < count(); ++i) {
        QRect rect = tabRect(i);
        
        QRect closeRect;
        closeRect.setSize(QSize(12, 12));
        closeRect.moveLeft(rect.right() - 16);
        closeRect.moveTop(rect.top() + (rect.height() - 12) / 2);
        
        painter.setPen(QPen(theme.fg, 1));
        painter.setRenderHint(QPainter::Antialiasing);
        
        painter.drawLine(closeRect.topLeft() + QPoint(2, 2), closeRect.bottomRight() - QPoint(2, 2));
        painter.drawLine(closeRect.topRight() + QPoint(-2, 2), closeRect.bottomLeft() + QPoint(2, -2));
    }
}

QSize TabBar::tabSizeHint(int index) const {
    QSize size = QTabBar::tabSizeHint(index);
    size.setWidth(qMin(size.width() + 20, 140));
    size.setHeight(24);
    return size;
}