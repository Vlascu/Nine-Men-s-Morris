#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>

class Logger {
public:
	static Logger& getInstance();

	log4cpp::Category& getRoot();

	~Logger();

private:
	Logger();

	log4cpp::PatternLayout* layoutPtr;
	log4cpp::FileAppender* fileAppenderPtr;
};