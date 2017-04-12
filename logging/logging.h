#ifndef LOGGING_H
#define LOGGING_H

#include <QString>

#include <iostream>
#include <fstream>
#include <memory>
#include "loghelper.h"

//=============================================================================================================
class VirtLogListener {
public:
    virtual void readLog(const std::string &logStr) = 0;
};
//=============================================================================================================
namespace logging {

    static bool consoleOut         = false;
    static bool viewFunctions      = false;
    static bool logOut             = false;
    static bool viewDebug          = false;
    static std::string flagsString = "ei";

    static std::string logDirName  = "";
    static std::string logFileName = "";

    static std::shared_ptr<VirtLogListener> listener  = nullptr;
    void setListener(const std::shared_ptr<VirtLogListener> value);
    std::shared_ptr<VirtLogListener> getListener();

    void setConsoleOut(bool value);
    bool getConsoleOut();

    void setLogOut(bool value);
    bool getLogOut();

    void setViewFunctions(bool value);
    bool getViewFunctions();

    void setFlagsString(const std::string &value);
    std::string getFlagsString();

    void setLogDirName(const std::string &value);
    std::string getLogDirName();

    void setLogFileName(const std::string &value);
    std::string getLogFileName();

    bool testAndCreateLogDir(std::string dirName);

    std::string getRealFunctionName(std::string functionName,
                                    std::string fileName,
                                    std::string elseString = "");

    void addStringLogFile(std::string str);
    bool createLogFile();

    void logPrint(std::string flag,
                  std::string realFunctionName,
                  const std::vector<std::string> *logVector);

    bool needToRerutn(std::string flag,
                      const std::string *str);

    std::vector<std::string> getLogVector(const std::string *str);

    void logPrint(std::string flag,
                  std::string functionName,
                  std::string fileName,
                  const std::string &str);

    namespace qt {
        void logPrint(std::string flag,
                      std::string functionName,
                      std::string fileName,
                      const QString &str);

    }

}
//=============================================================================================================

#endif // LOGGING_H
