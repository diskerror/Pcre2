
#include "Replacer.h"

using namespace Pcre2;

////////////////////////////////////////////////////////////////////////////////////////////////////
void Replacer::__construct(Php::Parameters& p)
{
	//			subjectLen		syntaxOption
	compile(p[0].buffer(), ((p.size() > 2) ? (int32_t) p[2] : 0));
	_replace = (const PCRE2_UCHAR*) (const char *) p[1].buffer();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Php::Value Replacer::replace(Php::Parameters &p) const
{
	uint32_t subjectLen = p[0].size();
	PCRE2_SIZE bufferSize = ( subjectLen < 2048 ) ? 4096 : (subjectLen*1.3);
	PCRE2_UCHAR outputBuffer[bufferSize];
	 
	int32_t erro = pcre2_substitute(
		_regex_compiled,
		(const PCRE2_UCHAR*) (const char *) p[0].buffer(),	//	subject,
		PCRE2_ZERO_TERMINATED,
		(( p.size() > 1 && (int32_t) p[1] > 0) ? (int32_t) p[1] : 0),	//	offset,
		PCRE2_SUBSTITUTE_GLOBAL,	//	options
		_match_data,
		NULL,		//	match context
		_replace,
		PCRE2_ZERO_TERMINATED,
		outputBuffer,
		&bufferSize
	);
	
	if ( erro < PCRE2_ERROR_NOMATCH ) {
		throw Exception( erro );
	}
	
	return Php::Value( (char*) outputBuffer, (int) bufferSize );
}
