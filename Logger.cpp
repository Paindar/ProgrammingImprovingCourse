#include <ctime>
#include <fstream>
#include <cstdlib>
#include "include\Logger.h"

/**
 * @brief Logger::createNewFile
 * Create a new log file after filename was set.
 * @return 0 if file was created, otherwise return 1;
 */
int Logger::CreateNewFile()
{
    std::ofstream file(_filepath);
    if(file.good())
    {

        file.close();
        return 0;
    }
    else
        FailToCreate();
    return 1;
}

std::string Logger::getDateTime()
{
	time_t tt = time(NULL);
	tm t;
	localtime_s(&t,&tt);
	char s[128] = {};
	sprintf_s(s,"%d.%d.%d %d.%d.%d",
		t.tm_year+1900,
		t.tm_mon,
		t.tm_mday,
		t.tm_hour,
		t.tm_min,
		t.tm_sec);
	return std::string(s);
}

/**
 * @brief Logger::Debug
 * to write message to log as debug message
 * @param msg what you want to write to log
 */
void Logger::Debug(const char* msg)
{
    if (_level>debug)
        return;
    Debug(std::string(msg));
}

/**
 * @brief Logger::Debug
 * to write message to log as debug message
 * @param msg what you want to write to log
 */
void Logger::Debug(std::string msg)
{
    if (_level>debug)
        return;
    std::ofstream file(_filepath,std::ios_base::app);
    if(file.good())
    {
        file<<"Debug\t";
        file<< getDateTime() <<"\t";
        file<<msg<<std::endl;
        file.close();
    }
    else
        FailToCreate();
}


/**
 * @brief Logger::Info
 * to write message to log as info message
 * @param msg what you want to write to log
 */
void Logger::Info(const char* msg)
{
    if (_level>info)
        return;
    Info(std::string(msg));
}

/**
 * @brief Logger::Info
 * to write message to log as info message
 * @param msg what you want to write to log
 */
void Logger::Info(std::string msg)
{
    if (_level>info)
        return;
    std::ofstream file(_filepath,std::ios_base::app);
    if(file.good())
    {
        file<<"Info\t";
        file<< getDateTime() <<"\t";
        file<<msg<<std::endl;
        file.close();
    }
    else
        FailToCreate();
}

/**
 * @brief Logger::Warning
 * to write message to log as Warning message
 * @param msg what you want to write to log
 */
void Logger::Warn(const char *msg)
{
    if (_level>warn)
        return;
    Warn(std::string(msg));
}

/**
 * @brief Logger::Warning
 * to write message to log as warning message
 * @param msg what you want to write to log
 */
void Logger::Warn(std::string msg)
{
    if (_level>warn)
        return;
    std::ofstream file(_filepath,std::ios_base::app);
    if(file.good())
    {
        file<<"Warning\t";
        file<< getDateTime() <<"\t";
        file<<msg<<std::endl;
        file.close();
    }
    else
        FailToCreate();
}

/**
 * @brief Logger::Error
 * to write message to log as error message
 * @param msg what you want to write to log
 */
void Logger::Error(const char *msg)
{
    if (_level>error)
        return;
    Debug(std::string(msg));
}

/**
 * @brief Logger::Error
 * to write message to log as error message
 * @param msg what you want to write to log
 */
void Logger::Error(std::string msg)
{
    if (_level>error)
        return;
    std::ofstream file(_filepath,std::ios_base::app);
    if(file.good())
    {
        file<<"Error\t";
        file<< getDateTime() <<"\t";
        file<<msg<<std::endl;
        file.close();
    }
    else
        FailToCreate();
}

/**
 * @brief Logger::Fatal
 * to write message to log as fatal message
 * @param msg what you want to write to log
 */
void Logger::Fatal(const char *msg)
{
    if (_level>fatal)
        return;
    Debug(std::string(msg));
}

/**
 * @brief Logger::Fatal
 * to write message to log as fatal message
 * @param msg what you want to write to log
 */
void Logger::Fatal(std::string msg)
{
    if (_level>fatal)
        return;
    std::ofstream file(_filepath,std::ios_base::app);
    if(file.good())
    {
        file<<"Fatal\t";
        file<< getDateTime() <<"\t";
        file<<msg<<std::endl;
        file.close();
    }
    else
        FailToCreate();
}

/**
 * @brief Logger::failToCreate
 */
void Logger::FailToCreate()
{
	exit(1);
}

Logger::Logger(): Logger(getDateTime())
{
}

Logger::Logger(std::string path)
{
    _filepath=path+".log";
    if(CreateNewFile()!=0)
    {
        FailToCreate();
    }
}

void Logger::SetMessageLevel(SafeLevel level)
{
    _level=level;
}

Logger::~Logger()
{
    //dtor
}
