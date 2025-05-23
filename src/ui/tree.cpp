#include "ui/tree.h"
#include "core/theme.h"
#include <QDir>
#include <QFileInfo>
#include <QHeaderView>

DirectoryTree::DirectoryTree(QWidget* parent) : QTreeWidget(parent) {
    setHeaderHidden(true);
    setRootIsDecorated(true);
    setAlternatingRowColors(false);
    
    auto& theme = Theme::instance();
    setFont(theme.font);
    
    QString style = QString(
        "QTreeWidget {"
        "background-color: %1;"
        "color: %2;"
        "border: none;"
        "outline: none;"
        "}"
        "QTreeWidget::item {"
        "padding: 2px;"
        "border: none;"
        "}"
        "QTreeWidget::item:selected {"
        "background-color: %3;"
        "}"
        "QTreeWidget::item:hover {"
        "background-color: %4;"
        "}"
    ).arg(theme.tree_bg.name())
     .arg(theme.fg.name())
     .arg(theme.accent.name())
     .arg(theme.border.name());
    
    setStyleSheet(style);
    
    connect(this, &QTreeWidget::itemDoubleClicked, this, &DirectoryTree::onItemDoubleClicked);
    connect(this, &QTreeWidget::itemExpanded, this, &DirectoryTree::onItemExpanded);
}

void DirectoryTree::setRootPath(const QString& path) {
    m_rootPath = path;
    clear();
    
    QFileInfo rootInfo(path);
    QTreeWidgetItem* rootItem = new QTreeWidgetItem(this);
    rootItem->setText(0, rootInfo.fileName().isEmpty() ? path : rootInfo.fileName());
    rootItem->setData(0, Qt::UserRole, path);
    rootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    
    populateTree(path, rootItem);
    rootItem->setExpanded(true);
}

void DirectoryTree::onItemDoubleClicked(QTreeWidgetItem* item, int column) {
    QString path = item->data(0, Qt::UserRole).toString();
    QFileInfo info(path);
    if (info.isFile()) {
        emit fileSelected(path);
    } else if (info.isDir()) {
        item->setExpanded(!item->isExpanded());
    }
}

void DirectoryTree::onItemExpanded(QTreeWidgetItem* item) {
    if (item->childCount() > 0 && item->child(0)->data(0, Qt::UserRole + 1).toBool()) {
        return;
    }
    
    while (item->childCount() > 0) {
        delete item->takeChild(0);
    }
    
    QString path = item->data(0, Qt::UserRole).toString();
    populateTree(path, item);
}

void DirectoryTree::populateTree(const QString& path, QTreeWidgetItem* parent) {
    QDir dir(path);
    if (!dir.exists()) return;
    
    QFileInfoList entries = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name);
    
    for (const QFileInfo& info : entries) {
        QTreeWidgetItem* item = new QTreeWidgetItem(parent);
        item->setText(0, info.fileName());
        item->setData(0, Qt::UserRole, info.absoluteFilePath());
        item->setData(0, Qt::UserRole + 1, true);
        
        if (info.isDir()) {
            QDir subDir(info.absoluteFilePath());
            if (!subDir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot).isEmpty()) {
                item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
            }
        }
    }
}