#include "loghelper.h"
//=============================================================================================================
void loghelper::delay(int sec) {
    std::this_thread::sleep_for(std::chrono::seconds(sec));
}
//-------------------------------------------------------------------------------------------------------------
void loghelper::mdelay(int msec) {
    std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}
//=============================================================================================================
void loghelper::strReplaceAll(std::string *str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while( (start_pos = str->find(from, start_pos)) != std::string::npos ) {
        str->replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}
//-------------------------------------------------------------------------------------------------------------
int loghelper::findLastPos(const std::string &str, const std::string& findStr) {
    int findPos = -1;
    size_t start_pos = 0;
    while( (start_pos = str.find(findStr, start_pos)) != std::string::npos ) {
        findPos = start_pos;
        start_pos += 1;
    }
    return findPos;
}
//-------------------------------------------------------------------------------------------------------------
bool loghelper::strStartWith(const std::string &str, const std::string& test) {
    if (str.length() < test.length() ) return false;
    return (str.compare(0, test.length(), test) == 0);
}
//-------------------------------------------------------------------------------------------------------------
bool loghelper::strEndWith(const std::string &str, const std::string& test) {
    if (str.length() < test.length() ) return false;
    return (str.compare(str.length() - test.length(), test.length(), test) == 0);
}
//=============================================================================================================
std::string loghelper::getBoolStr(const bool &val) {
   return val ? "true" : "false";
}
//=============================================================================================================
void loghelper::getRealPath(std::string *path) {
    if (path != nullptr) {
//#ifdef WIN32
        strReplaceAll(path, "\\", "/");
        if (strStartWith(*path, "/")) {
            *path = path->substr(1, path->length());
        }
//#endif
        if (!strEndWith(*path, "/")) {
            *path += "/";
        }
    }
}
//-------------------------------------------------------------------------------------------------------------
bool loghelper::testDir(std::string dirName, bool debugFlag) {

    getRealPath(&dirName);

    struct stat fileInfo;
    if( stat( dirName.c_str(), &fileInfo ) != 0 ) {
        if ( debugFlag ) {
            std::cerr << "testDir(). Cannot access to dirName == " << dirName;
            std::cerr << ". Error: " << strerror(errno) << std::endl;
            mdelay(5);
        }
        return false;
    }

    if ( (fileInfo.st_mode & S_IFMT) != S_IFDIR ) {
        if ( debugFlag ) {
            std::cerr << "testDir(). Error: \"" << dirName + "\" is not directiry.";
            mdelay(5);
        }
        return false;
    }

    if ( !(fileInfo.st_mode & S_IREAD) ) {
        if ( debugFlag ) {
            std::cerr << "testDir(). Error: \"" << dirName + "\" is not readable.";
            mdelay(5);
        }
        return false;
    }


    if ( !(fileInfo.st_mode & S_IWRITE) ) {
        if ( debugFlag ) {
            std::cerr << "testDir(). Error: \"" << dirName + "\" is not writable.";
            mdelay(5);
        }
        return false;
    }

    return true;
}
//-------------------------------------------------------------------------------------------------------------
bool loghelper::testAndCreateDir(std::string dirName) {

    getRealPath(&dirName);

    if (!testDir(dirName, false)) {
        if ( mkdir(dirName.c_str()) != 0 ) { // , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH
            std::cerr << "testAndCreateDir() mkdir() == 0 " << std::endl;
            mdelay(5);
            return false;
        }
    }

    if (!testDir(dirName)) {
        return false;
    }
    return true;
}
//=============================================================================================================
std::string loghelper::getNow() {
    //std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    //std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}
//=============================================================================================================
std::string loghelper::qt::convertQStringToStdString(const QString &str) {
    return str.toLocal8Bit().constData();
}
//-------------------------------------------------------------------------------------------------------------
QString loghelper::qt::convertStdStringToQString(const std::string &str) {
    //return QString::fromUtf8( logStr.data(), logStr.size() );
//    const char *charStr = str.c_str();
//    QByteArray ba(str.c_str());
//    QString res = QTextCodec::codecForName("UTF8")->toUnicode(ba);
//    res = QString::fromLocal8Bit(str.c_str());
//    return QString::fromLocal8Bit(str.c_str());
    return QTextCodec::codecForName("UTF8")->toUnicode(QByteArray(str.c_str()));
}
//-------------------------------------------------------------------------------------------------------------
void loghelper::qt::parseLogString(const std::string &logStr,
                                   QString &str,
                                   QString &dateStr,
                                   QString &functionNameStr,
                                   QString &type,
                                   QString &logFilterString) {

    str = convertStdStringToQString(logStr);

    dateStr = "";
    int pos1 = str.indexOf("|dt=");
    int pos2 = -1;
    if ( pos1 > -1 ) {
        pos2 = str.indexOf("|", pos1 + 1);
        if ( pos2 > -1 ) {
            dateStr = str.mid(pos1 + 4, pos2 - pos1 - 4);
            str = str.mid(0, pos1 + 1) + str.mid(pos2 + 1);
        }
    }

    functionNameStr = "";
    pos1 = str.indexOf("|fn=");
    if ( pos1 > -1 ) {
        pos2 = str.indexOf("|", pos1 + 1);
        if ( pos2 > -1 ) {
            functionNameStr = str.mid(pos1 + 4, pos2 - pos1 - 4);
            str = str.mid(0, pos1 + 1) + str.mid(pos2 + 1);
        }
    }

    logFilterString = "";
    pos1 = str.indexOf("|lf=");
    if ( pos1 > -1 ) {
        pos2 = str.indexOf("|", pos1 + 1);
        if ( pos2 > -1 ) {
            logFilterString = str.mid(pos1 + 4, pos2 - pos1 - 4);
            str = str.mid(0, pos1 + 1) + str.mid(pos2 + 1);
        }
    }

    type = "i";
    pos1 = str.indexOf("|");
    if (pos1 == 1) {
        type = str.mid(0, 1);
        str = str.mid(2);
    }


}
//=============================================================================================================

