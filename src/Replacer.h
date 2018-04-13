
#ifndef DISKERROR_PCRE2_REPLACER_H
#define DISKERROR_PCRE2_REPLACER_H

#include "Pcre2Base.h"
#include "flags/Replace.h"

class Replacer : public Pcre2Base
{
	std::string	_replacement;

public:
		Replacer() : Pcre2Base() {};
		
	virtual void		__construct(Php::Parameters&);

	virtual Php::Value setReplacement(Php::Parameters &);
	virtual Php::Value getReplacement(() const;

	virtual Php::Value	replace(Php::Parameters &) const;

	virtual void __destruct();
};

#endif	//	DISKERROR_PCRE2_REPLACER_H
