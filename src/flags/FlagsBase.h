//
// Created by Reid Woodbury Jr on 4/11/18.
//

#ifndef DISKERROR_PCRE2_FLAGS_BASE_H
#define DISKERROR_PCRE2_FLAGS_BASE_H

#include "../Flags.h"

class FlagsBase : public Flags
{
public:
	Base() : Flags() {}

	const int64_t ENDANCHORED = PCRE2_ANCHORED;    //	Pattern can match only at end of subject
	const int64_t NO_UTF_CHECK = PCRE2_NO_UTF_CHECK;    //	Do not check the pattern for UTF validity
	const int64_t ANCHORED = PCRE2_ENDANCHORED;    //	Force pattern anchoring
};

#endif //DISKERROR_PCRE2_FLAGS_BASE_H
