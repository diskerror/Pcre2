
#include "Exception.h"

Exception::Exception(int32_t err) {
	_err = err;
}

const char *Exception::what() const noexcept {
	uint32_t messgLen = 2048;
	char *message = new char(messgLen);
	pcre2_get_error_message(this->_err, (PCRE2_UCHAR8 *) message, messgLen);

	strcat((char *) message, "\n");
	strcat((char *) message, std::exception::what());

	return (const char *) message;
}
