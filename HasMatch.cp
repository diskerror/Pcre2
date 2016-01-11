
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
//	also used for "exec"
Php::Value HasMatch::__invoke(Php::Parameters& p) const
{
	if ( p.size() < 1 )
		throw Php::Exception( "First parameter to HasMatch::exec must be set." );
	
	const char* subject = (const char *) p[0];
	int32_t offset = ( p.size() > 1 ) ? (int32_t) p[1] : 0;
	
	//	do match
	int32_t matchCount = pcre2_match(
		this->_regex,
		(const PCRE2_UCHAR*) subject,
		PCRE2_ZERO_TERMINATED,
		offset,
		0,	//	options
		this->_match_data,
		NULL
	);
	
	if ( matchCount < PCRE2_ERROR_NOMATCH ) {
		throw new Exception( matchCount );
	}
	
	if ( matchCount == PCRE2_ERROR_NOMATCH )
		return false;
	
	return true;
}
