#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>

class Logger
{
public:
    static Logger& instance();
    static void logConsoleAlso(bool ifTrue);

    Logger& operator << (const std::string msg);
    Logger& operator << (const int val);
    Logger& operator << (const char* msg);

private:
    explicit Logger();
    ~Logger();

private:
    static bool              m_logConsole;
    static Logger*           m_logger;
    static std::ofstream*    m_logFile;
};

#endif // LOGGER_H
