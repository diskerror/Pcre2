
#ifndef DISKERROR_PCRE2_MATCHER_H
#define DISKERROR_PCRE2_MATCHER_H

#include "Pcre2Base.h"

class Matcher : public Pcre2Base
{
public:
		Matcher() : Pcre2Base() {};
		
	virtual Php::Value	hasMatch(Php::Parameters &) const;
	virtual Php::Value	match(Php::Parameters &) const;
	virtual Php::Value	matchAll(Php::Parameters &) const;

};

#endif	//	DISKERROR_PCRE2_MATCHER_H
