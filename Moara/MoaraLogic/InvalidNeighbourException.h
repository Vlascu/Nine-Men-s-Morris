#pragma once
#include <exception>

class InvalidNeighbourException : public std::exception
{
public:
	InvalidNeighbourException(const char* msg) : exception(msg) {};
};
