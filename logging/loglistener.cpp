#include "loglistener.h"
//=============================================================================================================
LogListener::LogListener() {
    run_thread  = nullptr;
    listener    = nullptr;
    running     = false;
    pauseMsec   = 50;
}
//-------------------------------------------------------------------------------------------------------------
LogListener::~LogListener() {
    stop();
    listener.reset();
}
//-------------------------------------------------------------------------------------------------------------
bool LogListener::start() {
    stop();
    running = true;
    run_thread = new std::thread(&LogListener::run, this);
    run_thread->detach();
}
//-------------------------------------------------------------------------------------------------------------
bool LogListener::stop() {
    if ( running ) {
        running = false;
        if (run_thread != nullptr) {
            std::this_thread::sleep_for(std::chrono::milliseconds(pauseMsec * 2));
            delete run_thread;
        }
    }
}
//-------------------------------------------------------------------------------------------------------------
void LogListener::run() {

    std::ifstream ifs;

    if ( logFileName != "" ) {
        ifs = std::ifstream(logFileName.c_str()); // , std::ios::ate
    }

    //http://stackoverflow.com/questions/2696976/implement-tail-f-in-c
    //http://www.cplusplus.com/forum/general/36649/
    //http://www.cplusplus.com/reference/istream/istream/seekg/

    //!!! http://stackoverflow.com/questions/11757304/how-to-read-a-growing-text-file-in-c

    //remember file position
    std::ios::streampos gpos = ifs.tellg();
    std::string line = "";
    while (running) {

        if ( ifs.is_open() ) {

            if(!std::getline(ifs, line) || ifs.eof()) {
                // if we fail, clear stream,
                // if end pos of file < gpos => start of the begining
                // else => return to beginning of line

                ifs.clear();
                ifs.seekg(0, std::ios::end);
                if ( ifs.tellg() < gpos ) {
                    //std::cerr << "LogListener::run(); ios_end_pos == " << ifs.tellg() << std::endl;
                    ifs.seekg(0, std::ios::beg);
                    gpos = ifs.tellg();
                    if (listener != nullptr) {
                        listener->clearLog();
                    }
                }
                else {
                    ifs.seekg(gpos);
                }

                // and wait to try again
                std::this_thread::sleep_for(std::chrono::milliseconds(pauseMsec));
                continue;
            }

            // remember the position of the next line in case
            // the next read fails
            gpos = ifs.tellg();
            //std::cerr << "LogListener::run(); gpos == " << gpos << std::endl;

            if (listener != nullptr) {
                listener->readLog(line);
            }
        }

        std::this_thread::sleep_for (std::chrono::milliseconds(pauseMsec));
    }

    if ( ifs.is_open() ) {
        ifs.close();
    }
    //std::cerr << "LogListener::run(). end" << std::endl;
}
//-------------------------------------------------------------------------------------------------------------
