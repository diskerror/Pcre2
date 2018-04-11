
#ifndef DISKERROR_PCRE2_REPLACER_H
#define DISKERROR_PCRE2_REPLACER_H

#include "Pcre2Base.h"

class Replacer : public Pcre2Base
{
	const PCRE2_UCHAR*	_replace;

public:
		Replacer() : Pcre2Base() {};
		
	virtual void		__construct(Php::Parameters&);
	
	virtual Php::Value	replace(Php::Parameters &) const;
	
};

#endif	//	DISKERROR_PCRE2_REPLACER_H
