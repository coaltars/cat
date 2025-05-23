#pragma once
#include <QMainWindow>
#include <QHBoxLayout>
#include <QSplitter>
#include "editor/editor.h"
#include "tree.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    
protected:
    void keyPressEvent(QKeyEvent* event) override;
    
private slots:
    void openFile();
    void openFolder();
    void saveAs();
    
private:
    void setupUI();
    void setupMenus();
    void setupShortcuts();
    
    QSplitter* m_splitter;
    DirectoryTree* m_tree;
    Editor* m_editor;
};