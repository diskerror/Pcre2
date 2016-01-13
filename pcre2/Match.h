
#ifndef DISKERROR_PCREMATCH_H
#define DISKERROR_PCREMATCH_H
#pragma once

#include "Pcre2.h"

namespace Pcre2 {

class Match : public Pcre2
{
	
public:
		Match() : Pcre2() {};
		
	virtual void	__construct(Php::Parameters&);
	
	virtual Php::Value	exec(Php::Parameters&) const;
	
};

}	//	namespace Pcre2

#endif	//	DISKERROR_PCREMATCH_H
