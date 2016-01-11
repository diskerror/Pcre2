
#include "Replace.h"

using namespace Pcre2;

////////////////////////////////////////////////////////////////////////////////////////////////////
void Replace::__construct(Php::Parameters& p)
{
	const char* expression = p[0].buffer();
	this->_replace = (const PCRE2_UCHAR*) (const char *) p[1].buffer();
	
	this->init(expression, (( p.size() > 2 ) ? (int32_t) p[2] : 0) /*syntaxOption*/);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	also used for "exec"
Php::Value Replace::__invoke(Php::Parameters& p) const
{
	auto pSize = p.size();
	
	if ( pSize < 1 ) {
		throw Php::Exception( "First parameter to Replace::exec must be set." );
	}
	
	uint32_t subjectLen = p[0].size();
	PCRE2_SIZE bufferSize = ( subjectLen < 2048 ) ? 4096 : (subjectLen*1.2);
	PCRE2_UCHAR outputBuffer[bufferSize];
	 
	int32_t erro = pcre2_substitute(
		this->_regex,
		(const PCRE2_UCHAR*) (const char *) p[0].rawValue(),	//	subject,
		PCRE2_ZERO_TERMINATED,
		(( pSize > 1 && (int32_t) p[1] > 0) ? (int32_t) p[1] : 0),	//	offset,
		0, //PCRE2_SUBSTITUTE_GLOBAL,	//	options
		this->_match_data,
		NULL,		//	match context
		this->_replace,
		PCRE2_ZERO_TERMINATED,
		outputBuffer,
		&bufferSize
	);
	
	if ( erro < PCRE2_ERROR_NOMATCH )
		throw new Exception( erro );
	
// 	if ( erro == PCRE2_ERROR_NOMATCH || erro == 0 )
// 		return subject;
	
	return Php::Value( (char*) outputBuffer, (int) bufferSize );
}
