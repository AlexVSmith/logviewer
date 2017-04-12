#ifndef LOGLISTENER_H
#define LOGLISTENER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <atomic>

#include "loghelper.h"

class VirtListener {
public:
    virtual void readLog(std::string logStr) = 0;
    virtual void clearLog() = 0;
};

class LogListener
{
public:
    LogListener();
    ~LogListener();

    void setLogFileName(const std::string &value) { logFileName = value; }
    void setListener(const std::shared_ptr<VirtListener> value) { listener = value; }
    void setPauseMsec(int value) { pauseMsec = value; }

    bool start();
    bool stop();

private:

    std::string logFileName = "";
    int pauseMsec;
    std::shared_ptr<VirtListener> listener;
    std::atomic<bool> running;
    std::thread *run_thread;

    void run();

};

#endif // LOGLISTENER_H
