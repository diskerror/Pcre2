
#include "MatchAll.h"


using namespace Pcre2;

/*
 *	Must rewrite to retrieve ALL matches and captures.
 */
void MatchAll::getMatches(int32_t matchCount, const char* subject, Php::Array* matches) const
{
// 	std::vector<std::vector<std::string>> vMatch;
// 	PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(this->_match_data);
// 	PCRE2_SIZE i;
// 	for (i = 0; i < (PCRE2_SIZE) matchCount; i++) {
// 		vMatch.emplace_back( (subject + ovector[2*i]), (ovector[2*i+1] - ovector[2*i]) );
// 	}
// 	
// 	*matches = (Php::Array) vMatch;
}