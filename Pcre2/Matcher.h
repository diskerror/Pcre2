
#ifndef DISKERROR_PCRE2_MATCHER_H
#define DISKERROR_PCRE2_MATCHER_H

#include "Base.h"

namespace Pcre2 {

class Matcher : public Base
{
protected:

	int32_t _basicMatch(Php::Parameters &);

public:
		Matcher() : Base() {};
		
	virtual Php::Value	hasMatch(Php::Parameters &) const;
	virtual Php::Value	match(Php::Parameters &) const;
	virtual Php::Value	matchAll(Php::Parameters &) const;

};

}	//	namespace Base

#endif	//	DISKERROR_PCRE2_MATCHER_H
