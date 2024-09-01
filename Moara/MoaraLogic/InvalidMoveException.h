#pragma once
#include <exception>

class InvalidMoveException : public std::exception
{
public:
	InvalidMoveException(const char* msg) : exception(msg) {};
};
