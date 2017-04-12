#ifndef LOGHELPER_H
#define LOGHELPER_H

#include <QString>
#include <QTextCodec>

#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>

#include <sys/stat.h>

namespace loghelper {

    void delay(int sec);
    void mdelay(int msec);

    void strReplaceAll(std::string *str, const std::string& from, const std::string& to);
    int findLastPos(const std::string &str, const std::string& findStr);
    bool strStartWith(const std::string &str, const std::string& test);
    bool strEndWith(const std::string &str, const std::string& test);

    std::string getBoolStr(const bool &val);

    void getRealPath(std::string *path);
    bool testDir(std::string dirName, bool debugFlag = true);
    bool testAndCreateDir(std::string dirName);

    std::string getNow();

    namespace qt {

        std::string convertQStringToStdString(const QString &str);
        QString convertStdStringToQString(const std::string &str);

        void parseLogString(const std::string &logStr,
                            QString &str,
                            QString &dateStr,
                            QString &functionNameStr,
                            QString &type,
                            QString &logFilterString
                            );
    }

}

#endif // LOGHELPER_H
