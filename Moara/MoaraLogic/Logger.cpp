#include "Logger.h"

Logger& Logger::getInstance() {
	static Logger instance;
	return instance;
}

log4cpp::Category& Logger::getRoot() {
	return log4cpp::Category::getRoot();
}

Logger::Logger() {
	layoutPtr = new log4cpp::PatternLayout();
	layoutPtr->setConversionPattern("%d [%p]: %m%n");

	fileAppenderPtr = new log4cpp::FileAppender("default", "../MoaraLogic/Log.txt");
	fileAppenderPtr->setLayout(layoutPtr);

	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.setPriority(log4cpp::Priority::INFO);
	root.setAppender(fileAppenderPtr);
}

Logger::~Logger() {
	log4cpp::Category::shutdown();
}