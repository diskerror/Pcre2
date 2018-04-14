
#include "Pcre2Exception.h"

Pcre2Exception::Pcre2Exception(int32_t err) : Php::Exception("") {
	const uint32_t messgLen = 1024;
	PCRE2_UCHAR8 message[messgLen];

	pcre2_get_error_message(err, message, messgLen);

	_message = (const char*)message;
}

