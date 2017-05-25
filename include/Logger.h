#ifndef LOGGER_H
#define LOGGER_H
#include <string>
class Logger
{
	
public:
    enum SafeLevel { debug, info, warn, error, fatal };
	/** Default constructor */
	Logger();
	Logger(std::string);
	void Debug(const char*);
	void Debug(std::string);
	void Info(const char *);
	void Info(std::string);
	void Warn(const char*);
	void Warn(std::string);
	void Error(const char*);
	void Error(std::string);
	void Fatal(const char*);
	void Fatal(std::string);
	void SetMessageLevel(SafeLevel);
	/** Default destructor */
	virtual ~Logger();
protected:
private:
	std::string _filepath;
	SafeLevel _level;

	int CreateNewFile();
	void FailToCreate();
	std::string getDateTime();
};

#endif // LOGGER_H
