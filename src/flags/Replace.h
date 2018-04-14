//
// Created by Reid Woodbury Jr on 4/11/18.
//

#ifndef DISKERROR_PCRE2_FLAGS_REPLACE_H
#define DISKERROR_PCRE2_FLAGS_REPLACE_H

#include "Match.h"

namespace Flags {

class Replace : public Match
{
public:
	Replace() : Match()
	{ add(GLOBAL); }

	//	Additional options available to replace method.
	static const int64_t GLOBAL = PCRE2_SUBSTITUTE_GLOBAL;  //	Replace all occurrences in the subject
	static const int64_t EXTENDED = PCRE2_SUBSTITUTE_EXTENDED;  //	Do extended replacement processing
	static const int64_t UNSET_EMPTY = PCRE2_SUBSTITUTE_UNSET_EMPTY;  //	Simple unset insert = empty string
	static const int64_t UNKNOWN_UNSET = PCRE2_SUBSTITUTE_UNKNOWN_UNSET;  //	Treat unknown group as unset
//	static const int64_t OVERFLOW_LENGTH = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH;  //	If overflow, compute needed length
};

}

#endif //DISKERROR_PCRE2_FLAGS_MATCH_H
