
#ifndef DISKERROR_PCRE2_REPLACER_H
#define DISKERROR_PCRE2_REPLACER_H

#include "Pcre2Base.h"

class Replacer : public Pcre2Base
{
	std::string _replacement;

public:
	explicit Replacer() : Pcre2Base(), _replacement("")
	{};

	void __construct(Php::Parameters &);

	void setReplacement(Php::Parameters &);
	Php::Value getReplacement() const;

	Php::Value replace(Php::Parameters &) const;

	void __destruct();
};

#endif    //	DISKERROR_PCRE2_REPLACER_H
