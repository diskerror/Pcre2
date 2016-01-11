
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
	
	//	also used for "exec"
	virtual Php::Value	__invoke(Php::Parameters&) const;
	
};

}

#endif	//	DISKERROR_PCREHASMATCH_H
