#pragma once
#include <exception>

class InvalidStateException : public std::exception
{
public:
	InvalidStateException(const char* msg) : exception(msg) {};
};
