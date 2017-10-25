#include "logger.h"
#include <string>
#include <iostream>

using namespace std;

/////////////////////////////////////////////////////////

bool            Logger::m_logConsole = true;
Logger*         Logger::m_logger = NULL;
std::ofstream*  Logger::m_logFile = NULL;

/////////////////////////////////////////////////////////

Logger& Logger::instance()
{
    if (m_logger == NULL)
        m_logger = new Logger();
    return *m_logger;
}

void Logger::logConsoleAlso(bool ifTrue)
{
    m_logConsole = ifTrue;
}

Logger& Logger::operator <<(string msg)
{
    if (m_logFile != NULL) {
        *m_logFile << msg;
        m_logFile->flush();

        if (m_logConsole)
            cout << msg;
    }
    return *this;
}

Logger &Logger::operator <<(const char *msg)
{
    if (m_logFile != NULL) {
        *m_logFile << std::string(msg);
        m_logFile->flush();

        if (m_logConsole)
            cout << msg;
    }
    return *this;
}

Logger& Logger::operator <<(int val)
{
    if (m_logFile != NULL) {
        *m_logFile << val;
        m_logFile->flush();

        if (m_logConsole)
            cout << val;
    }
    return *this;
}

Logger::Logger()
{
    m_logFile = new ofstream("./pcs_server_log.txt", ios_base::out | ios_base::app);
    if (!m_logFile->is_open())
        m_logFile = NULL;
}

Logger::~Logger()
{
    if (m_logFile != NULL) {
        m_logFile->close();
        delete m_logFile;
    }
}
