
#include "Replace.h"

using namespace Pcre2;

////////////////////////////////////////////////////////////////////////////////////////////////////
void Replace::__construct(Php::Parameters& p)
{
	//			subjectLen		syntaxOption
	this->init( p[0].buffer(), (( p.size() > 2 ) ? (int32_t) p[2] : 0) );
	this->_replace = (const PCRE2_UCHAR*) (const char *) p[1].buffer();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	also used for "exec"
Php::Value Replace::__invoke(Php::Parameters& p) const
{
	uint32_t subjectLen = p[0].size();
	PCRE2_SIZE bufferSize = ( subjectLen < 2048 ) ? 4096 : (subjectLen*1.3);
	PCRE2_UCHAR outputBuffer[bufferSize];
	 
	int32_t erro = pcre2_substitute(
		this->_regex,
		(const PCRE2_UCHAR*) (const char *) p[0].buffer(),	//	subject,
		PCRE2_ZERO_TERMINATED,
		(( p.size() > 1 && (int32_t) p[1] > 0) ? (int32_t) p[1] : 0),	//	offset,
		PCRE2_SUBSTITUTE_GLOBAL,	//	options
		this->_match_data,
		NULL,		//	match context
		this->_replace,
		PCRE2_ZERO_TERMINATED,
		outputBuffer,
		&bufferSize
	);
	
	if ( erro < PCRE2_ERROR_NOMATCH ) {
		throw new Exception( erro );
	}
	
	return Php::Value( (char*) outputBuffer, (int) bufferSize );
}
