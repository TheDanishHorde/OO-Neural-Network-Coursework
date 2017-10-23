// Contains handy new exceptions for use in other bits of code
#pragma once
#include <exception>

class too_large : public std::exception {
public:
	virtual const char* what() const throw()
	{
		return "Exception: Too large";
	}
};

class too_small : public std::exception{
public:	
	virtual const char* what() const throw()
	{
		return "Exception: Too small";
	}
};

class cin_fail : public std::exception {
public:
	virtual const char* what() const throw()
	{
		return "Exception: cin fail";
	}
};

class not_eof : public std::exception {
public:
	virtual const char* what() const throw()
	{
		return "Exception: Not all input accepted";
	}
};