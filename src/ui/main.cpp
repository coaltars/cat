#include "ui/main.h"
#include "core/theme.h"
#include <QKeyEvent>
#include <QApplication>
#include <QFileDialog>
#include <QDir>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    setupMenus();
    setupShortcuts();
    
    auto& theme = Theme::instance();
    setStyleSheet(QString("QMainWindow { background-color: %1; }").arg(theme.bg.name()));
}

void MainWindow::setupUI() {
    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_tree = new DirectoryTree(this);
    m_editor = new Editor(this);
    
    m_splitter->addWidget(m_tree);
    m_splitter->addWidget(m_editor);
    m_splitter->setSizes({1024, 1024});
    m_splitter->setHandleWidth(1);
    
    setCentralWidget(m_splitter);
    
    connect(m_tree, &DirectoryTree::fileSelected, m_editor, &Editor::openFile);
    
    m_tree->setRootPath(QDir::currentPath());
    m_editor->newFile();
}

void MainWindow::setupMenus() {
    auto& theme = Theme::instance();

    QString menuStyle = QString(
        "QMenuBar {"
        "background-color: %1;"
        "color: %2;"
        "border-bottom: 1px solid %3;"
        "}"
        "QMenuBar::item {"
        "background: transparent;"
        "padding: 4px 8px;"
        "}"
        "QMenuBar::item:selected {"
        "background: %3;"
        "}"
        "QMenu {"
        "background-color: %1;"
        "color: %2;"
        "border: 1px solid %3;"
        "}"
        "QMenu::item {"
        "padding: 4px 20px;"
        "}"
        "QMenu::item:selected {"
        "background-color: %4;"
        "}"
    ).arg(theme.bg.name())
     .arg(theme.fg.name())
     .arg(theme.border.name())
     .arg(theme.accent.name());
    
    menuBar()->setStyleSheet(menuStyle);
    
    QMenu* fileMenu = menuBar()->addMenu("&File");
    
    QAction* newAction = fileMenu->addAction("&New");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, m_editor, &Editor::newFile);
    
    QAction* openFileAction = fileMenu->addAction("&Open File...");
    openFileAction->setShortcut(QKeySequence::Open);
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);
    
    QAction* openFolderAction = fileMenu->addAction("Open &Folder...");
    openFolderAction->setShortcut(QKeySequence("Ctrl+Shift+O"));
    connect(openFolderAction, &QAction::triggered, this, &MainWindow::openFolder);
    
    fileMenu->addSeparator();
    
    QAction* saveAction = fileMenu->addAction("&Save");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, m_editor, &Editor::saveCurrentFile);
    
    QAction* saveAsAction = fileMenu->addAction("Save &As...");
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);
    
    fileMenu->addSeparator();
    
    QAction* closeTabAction = fileMenu->addAction("&Close Tab");
    closeTabAction->setShortcut(QKeySequence::Close);
    connect(closeTabAction, &QAction::triggered, m_editor, &Editor::closeCurrentTab);
    
    fileMenu->addSeparator();
    
    QAction* quitAction = fileMenu->addAction("&Quit");
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, QApplication::instance(), &QApplication::quit);
}

void MainWindow::openFile() {
    QString path = QFileDialog::getOpenFileName(this, "Open File", m_tree->getRootPath());
    if (!path.isEmpty()) {
        m_editor->openFile(path);
    }
}

void MainWindow::openFolder() {
    QString path = QFileDialog::getExistingDirectory(this, "Open Folder", m_tree->getRootPath());
    if (!path.isEmpty()) {
        m_tree->setRootPath(path);
    }
}

void MainWindow::saveAs() {
    QString path = QFileDialog::getSaveFileName(this, "Save As", m_tree->getRootPath());
    if (!path.isEmpty()) {
        m_editor->saveAs(path);
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->modifiers() & Qt::ControlModifier) {
        switch (event->key()) {
        case Qt::Key_N:
            m_editor->newFile();
            return;
        case Qt::Key_O:
            if (event->modifiers() & Qt::ShiftModifier) {
                openFolder();
            } else {
                openFile();
            }
            return;
        case Qt::Key_S:
            if (event->modifiers() & Qt::ShiftModifier) {
                saveAs();
            } else {
                m_editor->saveCurrentFile();
            }
            return;
        case Qt::Key_W:
            m_editor->closeCurrentTab();
            return;
        case Qt::Key_Q:
            QApplication::quit();
            return;
        }
    }
    QMainWindow::keyPressEvent(event);
}