#include "editor/editor.h"
#include "core/files.h"
#include <QFileInfo>

Editor::Editor(QWidget* parent) : QWidget(parent) {
    m_tabBar = new TabBar(this);
    m_stack = new QStackedWidget(this);
    m_layout = new QVBoxLayout(this);
    
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_tabBar);
    m_layout->addWidget(m_stack);
    
    connect(m_tabBar, &QTabBar::currentChanged, this, &Editor::onTabChanged);
    connect(m_tabBar, &TabBar::tabCloseRequested, this, &Editor::onTabCloseRequested);
}

void Editor::openFile(const QString& path) {
    QString content = FileManager::readFile(path);
    if (content.isEmpty() && !QFileInfo(path).exists()) return;
    
    TextArea* area = new TextArea(this);
    area->setContent(content);
    
    QFileInfo info(path);
    int index = addTab(info.fileName(), area);
    m_filePaths[index] = path;
    
    FileManager::addRecentFile(path);
}

void Editor::newFile() {
    TextArea* area = new TextArea(this);
    addTab("untitled", area);
}

void Editor::closeCurrentTab() {
    int index = m_tabBar->currentIndex();
    if (index >= 0) {
        onTabCloseRequested(index);
    }
}

void Editor::saveCurrentFile() {
    int index = m_tabBar->currentIndex();
    if (index < 0 || index >= m_filePaths.size()) return;
    
    QString path = m_filePaths[index];
    if (path.isEmpty()) return;
    
    TextArea* area = qobject_cast<TextArea*>(m_stack->widget(index));
    if (area) {
        FileManager::writeFile(path, area->getContent());
    }
}

void Editor::saveAs(const QString& path) {
    int index = m_tabBar->currentIndex();
    if (index < 0) return;
    
    TextArea* area = qobject_cast<TextArea*>(m_stack->widget(index));
    if (area) {
        if (FileManager::writeFile(path, area->getContent())) {
            m_filePaths[index] = path;
            QFileInfo info(path);
            m_tabBar->setTabText(index, info.fileName());
            FileManager::addRecentFile(path);
        }
    }
}

void Editor::onTabChanged(int index) {
    m_stack->setCurrentIndex(index);
}

void Editor::onTabCloseRequested(int index) {
    QWidget* widget = m_stack->widget(index);
    m_stack->removeWidget(widget);
    m_tabBar->removeTab(index);
    m_filePaths.removeAt(index);
    widget->deleteLater();
}

int Editor::addTab(const QString& title, TextArea* area) {
    int index = m_tabBar->addTab(title);
    m_stack->addWidget(area);
    m_tabBar->setCurrentIndex(index);
    m_filePaths.append(QString());
    return index;
}