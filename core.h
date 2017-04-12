#ifndef CORE_H
#define CORE_H

#include <QMainWindow>
#include <QTextEdit>
#include <QFileDialog>
#include <QtWidgets>

#include "logging.h"
#include "childlog.h"

namespace Ui {
class Core;
}

class Core;

class RealListener : public VirtLogListener {
private:
    Core *parent;
public:
    explicit RealListener(Core *core = 0) : parent(core)  {}
    ~RealListener() {
        delete parent;
    }
    void readLog(const std::string &logStr);
};

class Core : public QMainWindow
{
    Q_OBJECT
public:
    explicit Core(QWidget *parent = 0);
    ~Core();
    Ui::Core* getUi() {return ui;}
private:

    Ui::Core *ui;
    std::shared_ptr<RealListener> realListener;

    QMdiSubWindow *findMdiChild(const QString &fileName) const;
    ChildLog *createChildLog();

    bool loadFile(const QString &fileName);
    bool openFile(const QString &fileName);
    void open();
    void readSettings();
    void writeSettings();

/// recentFile

    enum { MaxRecentFiles = 5 };

    QAction *recentFileActs[MaxRecentFiles];
    QAction *recentFileSeparator;
    QAction *recentFileSubMenuAct;

    static bool hasRecentFiles();
    void prependToRecentFiles(const QString &fileName);
    void setRecentFilesVisible(bool visible);
    void updateRecentFileActions();
    void openRecentFile();

    void createActions();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

};

#endif // CORE_H
