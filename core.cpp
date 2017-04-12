#include "core.h"
#include "ui_core.h"
//=============================================================================================================
//-------------------------------------------------------------------------------------------------------------
void RealListener::readLog(const std::string &logStr) {

    QString str, dateStr, functionNameStr, type, logFilterString;
    loghelper::qt::parseLogString(logStr, str, dateStr, functionNameStr, type, logFilterString);

     QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
     str = codec->toUnicode(str.toUtf8());
//    logging::logPrint("d", __FUNCTION__, __FILE__, "RealListener::readLog(); logStr == " + logStr);
//    logging::qt::logPrint("d", __FUNCTION__, __FILE__, "RealListener::readLog(); str    == " + str);

//    std::cerr << "RealListener::readLog(). logStr          == " << logStr << std::endl;
    std::cerr << "RealListener::readLog(). str             == " << loghelper::qt::convertQStringToStdString(str) << std::endl;
//    std::cerr << "RealListener::readLog(). dateStr         == " << loghelper::qt::convertQStringToStdString(dateStr) << std::endl;
//    std::cerr << "RealListener::readLog(). functionNameStr == " << loghelper::qt::convertQStringToStdString(functionNameStr) << std::endl;
//    std::cerr << "RealListener::readLog(). type            == " << loghelper::qt::convertQStringToStdString(type) << std::endl;
//    loghelper::mdelay(5);

    if ( str != "" ) {

        QFont textFont("Courier New");
        textFont.setStyleHint(QFont::SansSerif);
        textFont.setItalic(false);
        textFont.setBold(false);
        textFont.setPixelSize(12);

        QColor foregroundColor = QColor(Qt::black);
        if (type == "e") {
            foregroundColor = QColor(Qt::red);
        }
        else if (type == "f") {
            foregroundColor = QColor(Qt::darkRed);
            textFont.setItalic(true);
        }
        else if (type == "w") {
            foregroundColor = QColor(Qt::darkMagenta);
        }
        else if (type == "d") {
            foregroundColor = QColor(Qt::darkGreen);
        }
        else if (type == "n") {
            foregroundColor = QColor(Qt::blue);
        }

        QTextCharFormat curFmt = parent->getUi()->teDebug->currentCharFormat();

        bool changeCharFormat = false;
        if (curFmt.foreground().color() != foregroundColor ) {
            changeCharFormat = true;
        }
        else if (curFmt.font().styleHint() != textFont.styleHint() ) {
            changeCharFormat = true;
        }
        else if (curFmt.font().pixelSize() != textFont.pixelSize() ) {
            changeCharFormat = true;
        }
        else if (curFmt.font().italic() != textFont.italic() ) {
            changeCharFormat = true;
        }
        else if (curFmt.font().bold() != textFont.bold() ) {
            changeCharFormat = true;
        }

        if (changeCharFormat) {
            QTextCharFormat fmt;
            fmt.setFont(textFont);
            fmt.setForeground(foregroundColor);
            parent->getUi()->teDebug->mergeCurrentCharFormat(fmt);
        }
        parent->getUi()->teDebug->append(str);
    }

}
//=============================================================================================================
Core::Core(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Core)
{
    ui->setupUi(this);

    connect( ui->actionDebugWindow, &QAction::triggered, this, [=](){
//        if (ui->actionDebugWindow->isChecked()) {
//            ui->dwDebug->setFloating(false);
//        }
        ui->dwDebug->setVisible(ui->actionDebugWindow->isChecked());
    });

    ui->actionDebugWindow->setChecked(true);
    ui->dwDebug->setVisible(ui->actionDebugWindow->isChecked());
    connect( ui->dwDebug,           &QDockWidget::visibilityChanged,    this, [=](){
        ui->actionDebugWindow->setChecked(ui->dwDebug->isVisible());
    } );

    connect(ui->actionOpen, &QAction::triggered, this, &Core::open);

    connect(ui->mdiArea, &QMdiArea::subWindowActivated, this, [=]() {
        qDebug() << "yes";
    });

    realListener.reset(new RealListener(this));
    logging::setListener(realListener);

    logging::qt::logPrint("d", __FUNCTION__, __FILE__, "Core::Core(); after logging::setListener(realListener);");
    logging::logPrint("n", __FUNCTION__, __FILE__, logging::getRealFunctionName(__FUNCTION__, __FILE__));

    createActions();
    readSettings();

    setWindowTitle(QString("Log viwer"));
    statusBar()->showMessage(QString("Ready"));
}
//-------------------------------------------------------------------------------------------------------------
Core::~Core() {
//    std::cerr << "Core::~Core(0);" << std::endl;
//    loghelper::mdelay(5);
    realListener.reset();
//    std::cerr << "Core::~Core(1);" << std::endl;
//    loghelper::mdelay(5);
    delete ui;
}
//-------------------------------------------------------------------------------------------------------------
void Core::closeEvent(QCloseEvent *event) {
    ui->mdiArea->closeAllSubWindows();
    if (ui->mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        writeSettings();
        event->accept();
    }
}

//-------------------------------------------------------------------------------------------------------------
QMdiSubWindow *Core::findMdiChild(const QString &fileName) const {
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();
    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList()) {
        ChildLog *childLog = qobject_cast<ChildLog *>(window->widget());
        if (childLog->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------------
ChildLog *Core::createChildLog() {
    ChildLog *child = new ChildLog;
    ui->mdiArea->addSubWindow(child);
//#ifndef QT_NO_CLIPBOARD
//    connect(child, &QTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
//    connect(child, &QTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
//#endif
    return child;
}
//-------------------------------------------------------------------------------------------------------------
bool Core::loadFile(const QString &fileName) {
    ChildLog *child = createChildLog();
    const bool succeeded = child->loadFile(fileName);
    if (succeeded)
        child->show();
    else
        child->close();
    Core::prependToRecentFiles(fileName);
    return succeeded;
}
//-------------------------------------------------------------------------------------------------------------
bool Core::openFile(const QString &fileName) {
    logging::logPrint("n", __FUNCTION__, __FILE__, logging::getRealFunctionName(__FUNCTION__, __FILE__));
    logging::logPrint("n", __FUNCTION__, __FILE__, u8"Александр");

    if (QMdiSubWindow *existing = findMdiChild(fileName)) {
        ui->mdiArea->setActiveSubWindow(existing);
        return true;
    }

    const bool succeeded = loadFile(fileName);
    if (succeeded)
        statusBar()->showMessage(tr("File loaded"), 2000);
    return succeeded;

}
//-------------------------------------------------------------------------------------------------------------
void Core::open() {
    logging::logPrint("n", __FUNCTION__, __FILE__, logging::getRealFunctionName(__FUNCTION__, __FILE__));
    const QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        logging::qt::logPrint("w", __FUNCTION__, __FILE__, "Core::open(); fileName == " + fileName);
        openFile(fileName);
    }
}
//-------------------------------------------------------------------------------------------------------------
void Core::readSettings() {
    logging::logPrint("n", __FUNCTION__, __FILE__, logging::getRealFunctionName(__FUNCTION__, __FILE__));
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }

}
//-------------------------------------------------------------------------------------------------------------
void Core::writeSettings() {
    logging::logPrint("n", __FUNCTION__, __FILE__, logging::getRealFunctionName(__FUNCTION__, __FILE__));
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}
//-------------------------------------------------------------------------------------------------------------
static inline QString recentFilesKey() { return QStringLiteral("recentFileList"); }
static inline QString fileKey() { return QStringLiteral("file"); }
//-------------------------------------------------------------------------------------------------------------
bool Core::hasRecentFiles() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const int count = settings.beginReadArray(recentFilesKey());
    settings.endArray();
    return count > 0;
}
//-------------------------------------------------------------------------------------------------------------
static QStringList readRecentFiles(QSettings &settings) {
    QStringList result;
    const int count = settings.beginReadArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        result.append(settings.value(fileKey()).toString());
    }
    settings.endArray();
    return result;
}
//-------------------------------------------------------------------------------------------------------------
static void writeRecentFiles(const QStringList &files, QSettings &settings) {
    const int count = files.size();
    settings.beginWriteArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        settings.setValue(fileKey(), files.at(i));
    }
    settings.endArray();
}
//-------------------------------------------------------------------------------------------------------------
void Core::prependToRecentFiles(const QString &fileName) {

    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList oldRecentFiles = readRecentFiles(settings);
    QStringList recentFiles = oldRecentFiles;
    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);
    if (oldRecentFiles != recentFiles) {
        writeRecentFiles(recentFiles, settings);
    }
    setRecentFilesVisible(!recentFiles.isEmpty());
}
//-------------------------------------------------------------------------------------------------------------
void Core::setRecentFilesVisible(bool visible) {
    recentFileSubMenuAct->setVisible(visible);
    recentFileSeparator->setVisible(visible);
}
//-------------------------------------------------------------------------------------------------------------
void Core::updateRecentFileActions() {

    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList recentFiles = readRecentFiles(settings);
    const int count = qMin(int(MaxRecentFiles), recentFiles.size());
    int i = 0;
    for ( ; i < count; ++i) {
        const QString fileName = QFileInfo(recentFiles.at(i)).fileName();
        recentFileActs[i]->setText(tr("&%1 %2").arg(i + 1).arg(fileName));
        recentFileActs[i]->setData(recentFiles.at(i));
        recentFileActs[i]->setVisible(true);
    }
    for ( ; i < MaxRecentFiles; ++i)
        recentFileActs[i]->setVisible(false);

}
//-------------------------------------------------------------------------------------------------------------
void Core::openRecentFile() {
    if (const QAction *action = qobject_cast<const QAction *>(sender())) {
        openFile(action->data().toString());
    }
}
//-------------------------------------------------------------------------------------------------------------
void Core::createActions() {

    QMenu *fileMenu = ui->menuFile;
    QMenu *recentMenu = new QMenu(QString("Recent..."));
    connect(recentMenu, &QMenu::aboutToShow, this, &Core::updateRecentFileActions);
    recentFileSubMenuAct = fileMenu->insertMenu(ui->actionExit, recentMenu);

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = recentMenu->addAction(QString(), this, &Core::openRecentFile);
        recentFileActs[i]->setVisible(false);
    }
    recentFileSeparator = fileMenu->insertSeparator(ui->actionExit);

    setRecentFilesVisible(Core::hasRecentFiles());

}
//-------------------------------------------------------------------------------------------------------------
