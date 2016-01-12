
#ifndef DISKERROR_PCREMATCH_H
#define DISKERROR_PCREMATCH_H
#pragma once

#include "Pcre2.h"

namespace Pcre2 {

class Match : public Pcre2
{
protected:
// 	void	getMatches(int32_t, const char*, Php::Value&) const;
	
public:
		Match() : Pcre2() {};
		
	virtual void	__construct(Php::Parameters&);
	
	//	also used for "exec"
	virtual Php::Value	__invoke(Php::Parameters&) const;
	
};

}

#endif	//	DISKERROR_PCREMATCH_H
