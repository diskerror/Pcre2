
#ifndef DISKERROR_PCRE2_EXCEPTION_H
#define DISKERROR_PCRE2_EXCEPTION_H


class Exception : public ::Php::Exception
{
	int32_t _err;

public:
	Exception(int32_t err);

	const char *what() const noexcept override;
};

#endif	//	DISKERROR_PCRE2_EXCEPTION_H
