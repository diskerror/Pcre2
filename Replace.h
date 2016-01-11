
#ifndef DISKERROR_PCREREPLACE_H
#define DISKERROR_PCREREPLACE_H
#pragma once

#include "Pcre2.h"

namespace Pcre2 {

class Replace : public Pcre2
{
	const PCRE2_UCHAR*	_replace;

public:
		Replace() : Pcre2() {};
		
	virtual void		__construct(Php::Parameters&);
	
	//	also used for "exec"
	virtual Php::Value	__invoke(Php::Parameters&) const;
	
};

}

#endif	//	DISKERROR_PCREREPLACE_H
