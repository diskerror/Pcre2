
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
//	also used for "exec"
Php::Value Match::__invoke(Php::Parameters& p) const
{
	if ( p.size() < 1 )
		throw Php::Exception( "First parameter to Match::exec (or MatchAll::exec) must be set." );
	
	const char* subject = (const char *) p[0];
	Php::Array* matches = (Php::Array*) &p[1];
	int32_t offset = ( p.size() > 2 ) ? (int32_t) p[2] : 0;
	
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
	
	while ( (*matches)[0].exists() ) {
		(*matches)[0].unset();
	}
	
	if ( matchCount == PCRE2_ERROR_NOMATCH ) {
		return false;
	}
	
	this->getMatches(matchCount, subject, matches);
	
	return true;
}

void Match::getMatches(int32_t matchCount, const char* subject, Php::Array* matches) const
{
	std::vector<std::string> vMatch;
	PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(this->_match_data);
	PCRE2_SIZE i;
	for (i = 0; i < (PCRE2_SIZE) matchCount; i++) {
		vMatch.emplace_back( (subject + ovector[2*i]), (ovector[2*i+1] - ovector[2*i]) );
	}
	
	*matches = (Php::Array) vMatch;
}