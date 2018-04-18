//
// Created by Reid Woodbury Jr on 4/11/18.
//

#ifndef DISKERROR_PCRE2_FLAGS_BASE_H
#define DISKERROR_PCRE2_FLAGS_BASE_H

namespace Flags {

class Base : public ::Php::Base
{
public:
	//  These flags are common to all.
	static const int64_t ENDANCHORED = PCRE2_ENDANCHORED;    //	Pattern can match only at end of subject
	static const int64_t NO_UTF_CHECK = PCRE2_NO_UTF_CHECK;    //	Do not check the pattern for UTF validity
	static const int64_t ANCHORED = PCRE2_ANCHORED;    //	Force pattern anchoring
};


}

#endif //DISKERROR_PCRE2_FLAGS_BASE_H
