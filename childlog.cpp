#include "childlog.h"
//=============================================================================================================
void ChildListener::clearLog() {
    QTextCursor cursor( parent->document() );
    cursor.select( QTextCursor::Document );
    cursor.removeSelectedText();
    cursor.clearSelection();
    cursor.movePosition( QTextCursor::Start );
    parent->setTextCursor( cursor );
}
//-------------------------------------------------------------------------------------------------------------
void ChildListener::readLog(std::string logStr) {
    //logging::logPrint("n", __FUNCTION__, __FILE__, logging::getRealFunctionName(__FUNCTION__, __FILE__));
    //logging::logPrint("d", __FUNCTION__, __FILE__, "ChildListener::readLog()); logStr == " + logStr);
    QString str, dateStr, functionNameStr, type, logFilterString;
    loghelper::qt::parseLogString(logStr, str, dateStr, functionNameStr, type, logFilterString);
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

        QTextCharFormat curFmt = parent->currentCharFormat();

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
            parent->mergeCurrentCharFormat(fmt);
        }
        parent->append(str);
    }

}
//=============================================================================================================
ChildLog::ChildLog() {
    setAttribute(Qt::WA_DeleteOnClose);
    realListener.reset(new ChildListener(this));
    logListener.reset(new LogListener());
    logListener->setListener(realListener);
    //setLocale();
}
//-------------------------------------------------------------------------------------------------------------
ChildLog::~ChildLog() {
//    logListener.reset();
//    realListener.reset();
}
//-------------------------------------------------------------------------------------------------------------
bool ChildLog::loadFile(const QString &fileName) {
//    QApplication::setOverrideCursor(Qt::WaitCursor);
//    QApplication::restoreOverrideCursor();
    if ( !QFileInfo(fileName).exists() ) {
        logging::qt::logPrint("e", __FUNCTION__, __FILE__, "ChildLog::loadFile(). Error: file not exist");
        return false;
    }
    //logging::qt::logPrint("d", __FUNCTION__, __FILE__, "ChildLog::loadFile();");
    logListener->setLogFileName(loghelper::qt::convertQStringToStdString(fileName));
    logListener->start();
    setCurrentFile(fileName);
    return true;
}
//-------------------------------------------------------------------------------------------------------------
QString ChildLog::userFriendlyCurrentFile() {
    return strippedName(curFile);
}
//-------------------------------------------------------------------------------------------------------------
void ChildLog::setCurrentFile(const QString &fileName) {
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}
//-------------------------------------------------------------------------------------------------------------
QString ChildLog::strippedName(const QString &fullFileName) {
    return QFileInfo(fullFileName).fileName();
}
//-------------------------------------------------------------------------------------------------------------
