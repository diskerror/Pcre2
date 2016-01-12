
#ifndef DISKERROR_PCREHASMATCH_H
#define DISKERROR_PCREHASMATCH_H
#pragma once

#include "Pcre2.h"

namespace Pcre2 {

class HasMatch : public Pcre2
{
	
public:
		HasMatch() : Pcre2() {};
		
	virtual void	__construct(Php::Parameters&);
	
	virtual Php::Value	exec(Php::Parameters&) const;
	
};

}	//	namespace Pcre2

#endif	//	DISKERROR_PCREHASMATCH_H
