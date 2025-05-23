#pragma once
#include <QTreeWidget>

class DirectoryTree : public QTreeWidget {
    Q_OBJECT
public:
    explicit DirectoryTree(QWidget* parent = nullptr);
    void setRootPath(const QString& path);
    QString getRootPath() const { return m_rootPath; }
    
signals:
    void fileSelected(const QString& path);
    
private slots:
    void onItemDoubleClicked(QTreeWidgetItem* item, int column);
    void onItemExpanded(QTreeWidgetItem* item);
    
private:
    void populateTree(const QString& path, QTreeWidgetItem* parent = nullptr);
    QString m_rootPath;
};