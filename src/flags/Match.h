//
// Created by Reid Woodbury Jr on 4/11/18.
//

#ifndef DISKERROR_PCRE2_FLAGS_MATCH_H
#define DISKERROR_PCRE2_FLAGS_MATCH_H

#include "Base.h"

namespace Flags {

class Match : public Base
{
public:
	Match() : Base()
	{ add(NOTEMPTY); }

	static const int64_t NOTBOL = PCRE2_NOTBOL;    //	Subject string is not the beginning of a line
	static const int64_t NOTEOL = PCRE2_NOTEOL;    //	Subject string is not the end of a line
	static const int64_t NOTEMPTY = PCRE2_NOTEMPTY;    //	An empty string is not a valid match
	static const int64_t NOTEMPTY_ATSTART = PCRE2_NOTEMPTY_ATSTART;    //	An empty string at the start is not valid
//	static const int64_t PARTIAL_SOFT = PCRE2_PARTIAL_SOFT;
//	static const int64_t PARTIAL_HARD = PCRE2_PARTIAL_HARD;
};

}

#endif //DISKERROR_PCRE2_FLAGS_MATCH_H
