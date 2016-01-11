
#ifndef DISKERROR_PCRE2_H
#define DISKERROR_PCRE2_H
#pragma once

#include <phpcpp.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>


namespace Pcre2 {

class Pcre2 : public Php::Base
{
protected:

	pcre2_code*			_regex;
	pcre2_match_data*	_match_data;

			Pcre2();
	
	void	init(const char*, uint32_t);
	
public:
	virtual ~Pcre2();
		
	virtual void	__construct(Php::Parameters&) = 0;
	
	virtual Php::Value	__invoke(Php::Parameters&) const = 0;

};

////////////////////////////////////////////////////////////////////////////////
class Exception : public std::exception
{
	int32_t			_err;
	unsigned char*	_message;

public:
		Exception(int32_t err);
		virtual ~Exception();
	
	const unsigned char* what();
};

}

#endif	//	DISKERROR_PCRE2_H
