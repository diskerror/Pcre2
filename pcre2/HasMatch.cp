
#include "HasMatch.h"

using namespace Pcre2;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
void HasMatch::__construct(Php::Parameters& p)
{
	const char* expression = (const char *) p[0];
	
	uint32_t syntaxOption = ( p.size() > 1 ) ? (int32_t) p[1] : 0;
	
	this->init(expression, syntaxOption);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Php::Value HasMatch::exec(Php::Parameters& p) const
{
	//	do match
	int32_t matchCount = pcre2_match(
		this->_regex,
		(const PCRE2_UCHAR*) (const char *) p[0].buffer(),	//	subject,
		PCRE2_ZERO_TERMINATED,
		(( p.size() > 1 && (int32_t) p[1] > 0) ? (int32_t) p[1] : 0),	//	offset,
		0,	//	options
		this->_match_data,
		NULL
	);
	
	if ( matchCount < PCRE2_ERROR_NOMATCH ) {
		throw Exception( matchCount );
	}
	
	if ( matchCount == PCRE2_ERROR_NOMATCH ) {
		return false;
	}
	
	return true;
}
