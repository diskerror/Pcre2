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
	Replace() : Match() {}

	//	Additional options available to replace method.
	const int64_t GLOBAL = PCRE2_SUBSTITUTE_GLOBAL;  //	Replace all occurrences in the subject
	const int64_t EXTENDED = PCRE2_SUBSTITUTE_EXTENDED;  //	Do extended replacement processing
	const int64_t UNSET_EMPTY = PCRE2_SUBSTITUTE_UNSET_EMPTY;  //	Simple unset insert = empty string
	const int64_t UNKNOWN_UNSET = PCRE2_SUBSTITUTE_UNKNOWN_UNSET;  //	Treat unknown group as unset
//	const int64_t OVERFLOW_LENGTH = PCRE2_SUBSTITUTE_OVERFLOW_LENGTH;  //	If overflow, compute needed length   ???
};

}

#endif //DISKERROR_PCRE2_FLAGS_MATCH_H
