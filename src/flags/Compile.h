//
// Created by Reid Woodbury Jr on 4/11/18.
//

#ifndef DISKERROR_PCRE2_FLAGS_COMPILE_H
#define DISKERROR_PCRE2_FLAGS_COMPILE_H

#include "Base.h"

namespace Flags {

class Compile : public Base
{
public:
	static const int64_t ALLOW_EMPTY_CLASS = PCRE2_ALLOW_EMPTY_CLASS;    //	Allow empty classes
	static const int64_t ALT_BSUX = PCRE2_ALT_BSUX;    //	Alternative handling of \u, \U, and \x
	static const int64_t AUTO_CALLOUT = PCRE2_AUTO_CALLOUT;    //	Compile automatic callouts
	static const int64_t CASELESS = PCRE2_CASELESS;    //	Do caseless matching
	static const int64_t DOLLAR_ENDONLY = PCRE2_DOLLAR_ENDONLY;    //	$ not to match newline at end
	static const int64_t DOTALL = PCRE2_DOTALL;    //	. matches anything including NL
	static const int64_t DUPNAMES = PCRE2_DUPNAMES;    //	Allow duplicate names for subpatterns
	static const int64_t EXTENDED = PCRE2_EXTENDED;    //	Ignore white space and # comments
	static const int64_t FIRSTLINE = PCRE2_FIRSTLINE;    //	Force matching to be before newline
	static const int64_t MATCH_UNSET_BACKREF = PCRE2_MATCH_UNSET_BACKREF;    //	Match unset back references
	static const int64_t MULTILINE = PCRE2_MULTILINE;    //	^ and $ match newlines within data
	static const int64_t NEVER_UCP = PCRE2_NEVER_UCP;    //	Lock out PCRE2_UCP, e.g. via (*UCP)
	static const int64_t NEVER_UTF = PCRE2_NEVER_UTF;    //	Lock out PCRE2_UTF, e.g. via (*UTF)
	static const int64_t NO_AUTO_CAPTURE = PCRE2_NO_AUTO_CAPTURE;    //	Disable numbered capturing parentheses
	static const int64_t NO_AUTO_POSSESS = PCRE2_NO_AUTO_POSSESS;    //	Disable auto-possessification
	static const int64_t NO_DOTSTAR_ANCHOR = PCRE2_NO_DOTSTAR_ANCHOR;    //	Disable automatic anchoring for .*
	static const int64_t NO_START_OPTIMIZE = PCRE2_NO_START_OPTIMIZE;    //	Disable match-time start optimizations
	static const int64_t UCP = PCRE2_UCP;    //	Use Unicode properties for \d, \w, etc.
	static const int64_t UNGREEDY = PCRE2_UNGREEDY;    //	Invert greediness of quantifiers
	static const int64_t UTF = PCRE2_UTF;    //	Treat pattern and subjects as UTF strings
	static const int64_t NEVER_BACKSLASH_C = PCRE2_NEVER_BACKSLASH_C;    //	Lock out the use of \C in patterns
	static const int64_t ALT_CIRCUMFLEX = PCRE2_ALT_CIRCUMFLEX;    //	Alternative handling of ^ in multiline mode
	static const int64_t ALT_VERBNAMES = PCRE2_ALT_VERBNAMES;    //	Process backslashes in verb names
	static const int64_t USE_OFFSET_LIMIT = PCRE2_USE_OFFSET_LIMIT;    //	Enable offset limit for unanchored matching
	static const int64_t EXTENDED_MORE = PCRE2_EXTENDED_MORE;
	static const int64_t LITERAL = PCRE2_LITERAL;    //	Pattern characters are all literal
	static const int64_t DO_JIT = 0x0000000100000000;    //	Do more optimizing with JIT compiler.
};

}

#endif //DISKERROR_PCRE2_FLAGS_COMPILE_H
