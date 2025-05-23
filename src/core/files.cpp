#include "core/files.h"
#include <QFile>
#include <QTextStream>
#include <QSettings>

QString FileManager::readFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();
    
    QTextStream in(&file);
    return in.readAll();
}

bool FileManager::writeFile(const QString& path, const QString& content) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    
    QTextStream out(&file);
    out << content;
    return true;
}

QStringList FileManager::getRecentFiles() {
    QSettings settings;
    return settings.value("recentFiles").toStringList();
}

void FileManager::addRecentFile(const QString& path) {
    QSettings settings;
    QStringList files = getRecentFiles();
    files.removeAll(path);
    files.prepend(path);
    if (files.size() > 10) files = files.mid(0, 10);
    settings.setValue("recentFiles", files);
}
