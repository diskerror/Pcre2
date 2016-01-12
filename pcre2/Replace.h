
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
	
	virtual Php::Value	exec(Php::Parameters&) const;
	
};

}	//	namespace Pcre2

#endif	//	DISKERROR_PCREREPLACE_H
