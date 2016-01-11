
#include "Pcre2.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
Pcre2::Pcre2::Pcre2()
{
	this->_regex = NULL;
	this->_match_data = NULL;
}

void Pcre2::Pcre2::init(const char* expression, uint32_t syntaxOption)
{
	int errorcode;
	PCRE2_SIZE erroroffset;
	
	this->_regex = pcre2_compile(
		(PCRE2_SPTR) expression,
		PCRE2_ZERO_TERMINATED,
		syntaxOption,
		&errorcode,
		&erroroffset,
		NULL	//	match context
	);
	
	if ( this->_regex == NULL )
		throw Exception( errorcode );
	
	pcre2_jit_compile(this->_regex, PCRE2_JIT_COMPLETE);

	this->_match_data = pcre2_match_data_create_from_pattern(this->_regex, NULL);
}

Pcre2::Pcre2::~Pcre2()
{
	pcre2_match_data_free(this->_match_data);
	pcre2_code_free(this->_regex);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Pcre2::Exception::Exception(int32_t err)
{
	this->_err = err;
	this->_message = NULL;
}

Pcre2::Exception::~Exception()
{
	if ( this->_message != NULL )
		delete this->_message;
}
	
const unsigned char* Pcre2::Exception::what()
{
	if ( this->_message == NULL ) {
		uint32_t messgLen = 2048;
		this->_message = new unsigned char(messgLen);
		pcre2_get_error_message(this->_err, (PCRE2_UCHAR8*) this->_message, messgLen);
	
		strcat( (char*) this->_message, "\n" );
		strcat( (char*) this->_message, std::exception::what() );
	}
	
	return (const unsigned char*) this->_message;
}
