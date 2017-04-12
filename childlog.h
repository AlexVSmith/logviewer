#ifndef CHILDLOG_H
#define CHILDLOG_H

#include <QTextEdit>
#include <QFile>
#include <QFileInfo>

#include "logging.h"
#include "loglistener.h"

class ChildLog;

class ChildListener : public VirtListener {
private:
    ChildLog *parent;
public:
    explicit ChildListener(ChildLog *childLog) : parent(childLog)  {}
    void clearLog();
    void readLog(std::string logStr);
};

class ChildLog : public QTextEdit
{

    Q_OBJECT

private:
    QString curFile;

    std::shared_ptr<LogListener>   logListener;
    std::shared_ptr<ChildListener> realListener;

public:

    ChildLog();
    ~ChildLog();

    QString currentFile() { return curFile; }
    bool loadFile(const QString &fileName);

    QString userFriendlyCurrentFile();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

};

#endif // CHILDLOG_H
