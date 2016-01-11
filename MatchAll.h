
#ifndef DISKERROR_PCREMATCHALL_H
#define DISKERROR_PCREMATCHALL_H
#pragma once

#include "Match.h"

namespace Pcre2 {

class MatchAll : public Match
{
protected:
	void	getMatches(int32_t, const char*, Php::Array*) const;
	
public:
		MatchAll() : Match() {};
		
// 	virtual void	__construct(Php::Parameters&);
	
// 	virtual Php::Value	__invoke(Php::Parameters&) const;
	
};

}

#endif	//	DISKERROR_PCREMATCHALL_H
