#include "logging.h"
//=============================================================================================================
void logging::setConsoleOut(bool value) {
    consoleOut = value;
}
//-------------------------------------------------------------------------------------------------------------
bool logging::getConsoleOut() {
    return consoleOut;
}
//-------------------------------------------------------------------------------------------------------------
void logging::setLogOut(bool value) {
    logOut = value;
}
//-------------------------------------------------------------------------------------------------------------
bool logging::getLogOut() {
    return logOut;
}
//-------------------------------------------------------------------------------------------------------------
void logging::setViewFunctions(bool value) {
    viewFunctions = value;
}
//-------------------------------------------------------------------------------------------------------------
bool logging::getViewFunctions() {
    return viewFunctions;
}
//-------------------------------------------------------------------------------------------------------------
void logging::setFlagsString(const std::string &value) {
    if (value == "all") {
        flagsString = "abcdefjhijklmnopqrstuvwxyz";
    }
    else if (value == "none") {
        flagsString = "";
    }
    else {
        flagsString = value;
    }
}
//-------------------------------------------------------------------------------------------------------------
std::string logging::getFlagsString() {
    return flagsString;
}
//-------------------------------------------------------------------------------------------------------------
void logging::setLogDirName(const std::string &value) {
    logDirName = value;
}
//-------------------------------------------------------------------------------------------------------------
std::string logging::getLogDirName() {
    return logDirName;
}
//-------------------------------------------------------------------------------------------------------------
void logging::setLogFileName(const std::string &value) {
    logFileName = value;
}
//-------------------------------------------------------------------------------------------------------------
std::string logging::getLogFileName() {
    return logFileName;
}
//=============================================================================================================
bool logging::testAndCreateLogDir(std::string dirName) {

    loghelper::getRealPath(&dirName);

    if ( !loghelper::testDir(dirName) ) return false;

    if ( !loghelper::testAndCreateDir(dirName + ".log/") ) {
        return false;
    }

    setLogDirName(dirName + ".log/");

    return true;
}
//-------------------------------------------------------------------------------------------------------------
std::string logging::getRealFunctionName(std::string functionName,
                                std::string fileName,
                                std::string elseString) {

    loghelper::strReplaceAll(&fileName, "\\", "/");

    int posSlash = loghelper::findLastPos(fileName, "/");
    std::string realFunctionName = fileName.substr(posSlash + 1, fileName.length());

    if ( elseString == "" ) {
        realFunctionName += "." + functionName + "()";
    }
    else {
        realFunctionName += "." + functionName + "(" +elseString + ")";
    }

    return realFunctionName;
}
//-------------------------------------------------------------------------------------------------------------
void logging::addStringLogFile(std::string str) {
    if ( logDirName == "" ) return;
    std::string fullLogFileName = logDirName + logFileName;
    if (std::ifstream(fullLogFileName)) {
        std::fstream logFile;
        logFile.open(fullLogFileName, std::fstream::app);
        logFile << str << std::endl;
        logFile.close();
    }
}
//-------------------------------------------------------------------------------------------------------------
bool logging::createLogFile() {
    if ( logDirName == "" ) return false;

    std::string fullLogFileName = logDirName + logFileName;

    if (std::ifstream(fullLogFileName)) {
         if( std::remove(fullLogFileName.c_str()) != 0 ) {
             std::cerr << "createLogFile(). Error removing log file" << std::endl;
             loghelper::mdelay(5);
             return false;
         }
    }

    std::fstream logFile;
    logFile.open(fullLogFileName, std::fstream::in | std::fstream::out | std::fstream::app);
    logFile.close();

    if (!std::ifstream(fullLogFileName)) {
         std::cerr << "createLogFile(). Error creating log file" << std::endl;
         logDirName = "";
         loghelper::mdelay(5);
         return false;
    }

    setFlagsString(flagsString);

    std::string realFunctionName = getRealFunctionName(__FUNCTION__, __FILE__);
    addStringLogFile("i|dt=" + loghelper::getNow() + "|fn=" + realFunctionName + "|" + "lf=" + flagsString + "|");

    return true;
}
//-------------------------------------------------------------------------------------------------------------
void logging::logPrint(std::string flag,
                       std::string realFunctionName,
                       const std::vector<std::string> *logVector) {

//    std::string fullLogFileName = logDirName + logFileName;
//    std::cout << "logPrint(). flag             == " << flag << std::endl;
//    std::cout << "logPrint(). flagsString      == " << flagsString << std::endl;
//    std::cout << "logPrint(). realFunctionName == " << realFunctionName << std::endl;
//    std::cout << "logPrint(). fullLogFileName  == " << fullLogFileName << std::endl;
//    std::cout << "logPrint(). consoleOut       == " << consoleOut << std::endl;
//    std::cout << "logPrint(). logOut           == " << logOut << std::endl;
//    std::cout << "logPrint(). viewFunctions    == " << viewFunctions << std::endl;

    for ( std::string str : (*logVector)) {
        //std::cout << "logPrint(). logVector.str == " << str << std::endl;
        if ( listener != nullptr ) {
            //std::cout << "logPrint(). listener != nullptr; before listener->readLog(); " << std::endl;
            listener->readLog(flag + "|dt=" + loghelper::getNow() + "|fn=" + realFunctionName + "|" + str);
            //std::cout << "logPrint(). listener != nullptr; after listener->readLog(); " << std::endl;
        }
        if ( consoleOut ) {
            std::string out = str;
            if (viewFunctions) {
                out = realFunctionName + ". " + out;
            }
            if (flag == "w" || flag == "e" || flag == "f") {
                std::cerr << out << std::endl;
            }
            else {
                std::cout << out << std::endl;
            }
            loghelper::mdelay(5);
        }
        if ( logOut ) {
            addStringLogFile(flag + "|dt=" + loghelper::getNow() + "|fn=" + realFunctionName + "|" + str);
        }
    }
}
//-------------------------------------------------------------------------------------------------------------
bool logging::needToRerutn(std::string flag,
                           const std::string *str) {
    if ( (logFileName == "") && !consoleOut && (listener == nullptr) ) return true;
    if (flagsString == "") return true;
    if (*str == "") return true;
    if (flagsString.find(flag) == std::string::npos) return true;
    return false;
}
//-------------------------------------------------------------------------------------------------------------
std::vector<std::string> logging::getLogVector(const std::string *str) {
    std::vector<std::string> logVector;
    size_t start_pos = 0;
    size_t end_pos   = -1;
    while( (end_pos = str->find("\n", start_pos)) != std::string::npos ) {
        logVector.push_back(str->substr(start_pos, end_pos - start_pos));
        start_pos = end_pos + 1;
    }
    if ( start_pos < str->length() ) {
        logVector.push_back(str->substr(start_pos, str->length() - start_pos));
    }
    return logVector;
}
//-------------------------------------------------------------------------------------------------------------
void logging::logPrint(std::string flag,
              std::string functionName,
              std::string fileName,
              const std::string &str) {

    if ( needToRerutn(flag, &str) ) return;

    std::string realFunctionName = getRealFunctionName(functionName, fileName);
    std::vector<std::string> logVector = getLogVector(&str);

    logPrint(flag, realFunctionName, &logVector);

}
//=============================================================================================================
void logging::qt::logPrint(std::string flag,
              std::string functionName,
              std::string fileName,
              const QString &str) {
    std::string stdStr = loghelper::qt::convertQStringToStdString(str);
    logging::logPrint(flag, functionName, fileName, stdStr);
}
//=============================================================================================================
void logging::setListener(const std::shared_ptr<VirtLogListener> value) {
    listener = value;
}
//-------------------------------------------------------------------------------------------------------------
std::shared_ptr<VirtLogListener> logging::getListener() {
    return listener;
}
//=============================================================================================================
