#pragma once
#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "tab.h"
#include "text.h"

class Editor : public QWidget {
    Q_OBJECT
public:
    explicit Editor(QWidget* parent = nullptr);
    void openFile(const QString& path);
    void newFile();
    void closeCurrentTab();
    void saveCurrentFile();
    void saveAs(const QString& path);
    
private slots:
    void onTabChanged(int index);
    void onTabCloseRequested(int index);
    
private:
    TabBar* m_tabBar;
    QStackedWidget* m_stack;
    QVBoxLayout* m_layout;
    QStringList m_filePaths;
    
    int addTab(const QString& title, TextArea* area);
};