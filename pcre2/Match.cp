
#include "Match.h"

using namespace Pcre2;

////////////////////////////////////////////////////////////////////////////////////////////////////
void Match::__construct(Php::Parameters& p)
{
	const char* expression = (const char *) p[0];
	
	uint32_t syntaxOption = ( p.size() > 1 ) ? (int32_t) p[1] : 0;
	
	this->init(expression, syntaxOption);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Php::Value Match::exec(Php::Parameters& p) const
{
	const char* subject = (const char *) p[0];
// 	Php::Value* matches = (Php::Value*) &p[1];
	
	//	do match
	int32_t matchCount = pcre2_match(
		this->_regex,
		(const PCRE2_UCHAR*) subject,
		PCRE2_ZERO_TERMINATED,
		(( p.size() > 2 && (int32_t) p[2] > 0) ? (int32_t) p[2] : 0),	//	offset,
		0,	//	options
		this->_match_data,
		NULL
	);
	
	if ( matchCount < PCRE2_ERROR_NOMATCH ) {
		throw new Exception( matchCount );
	}
	
	if ( matchCount == PCRE2_ERROR_NOMATCH ) {
		return false;
	}
	
	//	Match first.
	if ( 1 ) {
		PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(this->_match_data);
		PCRE2_SIZE i;
		for (i = 0; i < (PCRE2_SIZE) matchCount; i++) {
	 		p[1][i] = Php::Value( (char*)(subject + ovector[2*i]), (ovector[2*i+1] - ovector[2*i]) );
		}
	}
	//	Match all.
// 	else {
// 	}
	
	return true;
}
 