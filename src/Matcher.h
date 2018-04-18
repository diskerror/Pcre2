
#ifndef DISKERROR_PCRE2_MATCHER_H
#define DISKERROR_PCRE2_MATCHER_H

#include "Pcre2Base.h"

class Matcher : public Pcre2Base
{
public:
		Matcher() : Pcre2Base() {};
		
	Php::Value	hasMatch(Php::Parameters &) const;
	Php::Value	match(Php::Parameters &) const;
	Php::Value	matchAll(Php::Parameters &) const;

};

#endif	//	DISKERROR_PCRE2_MATCHER_H
