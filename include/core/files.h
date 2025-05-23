#pragma once
#include <QString>
#include <QStringList>

class FileManager {
public:
    static QString readFile(const QString& path);
    static bool writeFile(const QString& path, const QString& content);
    static QStringList getRecentFiles();
    static void addRecentFile(const QString& path);
};