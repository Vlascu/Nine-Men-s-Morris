#pragma once
#include <exception>

class FileException : public std::exception
{
public:
	FileException(const char* msg) : exception(msg) {};
};