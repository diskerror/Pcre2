
#ifndef DISKERROR_PCRE2_REPLACER_H
#define DISKERROR_PCRE2_REPLACER_H

#include "Base.h"

namespace Pcre2 {

class Replacer : public Base
{
	const PCRE2_UCHAR*	_replace;

public:
		Replacer() : Base() {};
		
	virtual void		__construct(Php::Parameters&);
	
	virtual Php::Value	replace(Php::Parameters &) const;
	
};

}	//	namespace Pcre2

#endif	//	DISKERROR_PCRE2_REPLACER_H
